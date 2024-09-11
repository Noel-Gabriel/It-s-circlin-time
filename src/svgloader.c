#define NANOSVG_IMPLEMENTATION
#include "extern/nanosvg.h"

#include "src/svgloader.h"
#include "src/darray.h"
#include "src/utils.h"

#include <stdbool.h>
#include <string.h>
#include <raylib.h>

// Check for svg format
bool svgloader_is_svg(const char* path) {
    if(path == NULL) { 
        return false;
    }
    const char* ext = strrchr(path, '.');
    if(ext != NULL) {
        return strcmp(ext, ".svg") == 0;
    }
    return false;
}

// sample points given the control points of a cubic bezier curve 
void svgloader_sample_cubic_bezier(DArray* darray, double dt, Vector2* vs) {

    double t = 0.0;
    while(t < 1) {
        Vector2 b0 = utils_vector_scale(vs[0], (1-t)*(1-t)*(1-t));
        Vector2 b1 = utils_vector_scale(vs[1], 3*(1-t)*(1-t)*t);
        Vector2 b2 = utils_vector_scale(vs[2], 3*(1-t)*t*t);
        Vector2 b3 = utils_vector_scale(vs[3], t*t*t);

        Vector2 sample = utils_vector_add(b0,
                utils_vector_add(b1,
                utils_vector_add(b2, b3)));

        darray_insert(darray, sample);
        t += dt;
    }
}

float svgloader_scale_to_fit(float orig_width, float orig_height, float svg_width,
        float svg_height) {
    float x_scale = orig_width/svg_width;
    float y_scale = orig_height/svg_height;

    return (x_scale < y_scale)? x_scale : y_scale;
}


DArray* svgloader_sample_svg(const char* path, double dt, float width, float height) { 
    // only svg and dt between 0 and 1
    if(!svgloader_is_svg(path) || dt <= 0.0 || dt > 1) {
        return NULL;
    }
    
    NSVGimage* image = nsvgParseFromFile(path, "px", 96.0f);
    if(image == NULL) { return NULL; }

    printf("LOADED %s\n", path);

    // scale to fit given width and height, maintaining ratios and center
    float scale = svgloader_scale_to_fit(width, height, image->width, image->height);

    float x_offset = (width - image->width * scale) / 2.0f;
    float y_offset = (height - image->height * scale) / 2.0f;
    
    printf("SCALE: %f\n", scale);
    printf("SVG WIDTH: %f\n", image->width);
    printf("NEW SVG WIDTH: %f\n", image->width * scale);
    printf("SVG HEIGHT: %f\n", image->height);
    printf("NEW SVG HEIGHT: %f\n", image->height * scale);
    // go through all paths given as cubic bezier curves by nanoSVG
    DArray* sampled_points = darray_create();
    for(NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
        for(NSVGpath* path = shape->paths; path != NULL; path = path->next) {
            for(int i = 0; i < path->npts-1; i += 3) {
                float* points = &(path->pts[i*2]);
                // scale
                Vector2 vectors[4];
                for(int p = 0; p < 8; p += 2) {
                    Vector2 vector = {
                        points[p] * scale + x_offset,
                        points[p+1] * scale + y_offset
                    };
                    vectors[p / 2] = vector;
                }
                svgloader_sample_cubic_bezier(sampled_points, dt, vectors);
            }
        }
    }
    nsvgDelete(image);
    return sampled_points;
}
