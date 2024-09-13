#include "src/circle.h"
#include "src/darray.h"
#include "src/core.h"

#include <stdio.h>
#include <math.h>
#include <raylib.h>

// Color of circles drawn on screen
#define CIRCLE_COLOR GREEN
// Color of the vector (radius) of each circle
#define VECTOR_COLOR RED 
// Thickness of each vector
#define VECTOR_THICKNESS 2.0f
// Length of the trace effect of past results
#define CIRCLE_TRACE_LENGTH 70
// Color of the trace effect of past results
#define CIRCLE_TRACE_COLOR YELLOW
// Thickness of the trace effect of past results
#define CIRCLE_TRACE_THICKNESS 5.0f

void circle_free(FourierSeries* fs) {
    if(fs == NULL || fs->circles == NULL) { return; }

    free(fs->circles);
    fs->circles = NULL;
    fs->num_circles = 0;
    dllist_free(fs->last_results);
    fs->last_results = NULL;
}

void circle_create(DArray* trace, size_t num_coeffs, double dt, FourierSeries* fs) {
    circle_free(fs);

    Complex* coefficients = core_fourier_coeffs(trace, dt, num_coeffs);

    fs->circles = malloc(num_coeffs * sizeof(Circle));
    fs->num_circles = num_coeffs;
    fs->last_results = dllist_create();

    for(int i = 0; i < num_coeffs; ++i) {
        // alternate frequencies; 0, 1, -1, 2, -2, ...
        int n = (i % 2 == 0)? -1 * i / 2 : (i + 1) / 2;
        Circle c = {coefficients[i], (n * 2 * PI)};
        fs->circles[i] = c;
    }
    free(coefficients);
}

void circle_step(FourierSeries* fs, double angle_update) {
    if(fs == NULL || fs->circles == NULL) { return; }

    for(int i = 0; i < fs->num_circles; ++i) {
        double freq = fs->circles[i].freq * angle_update;
        // Rotation on the complex plane = multiplication :)
        Complex e = {cos(freq), sin(freq)};
        fs->circles[i].c = complex_mult(fs->circles[i].c, e);
    }
}

void circle_draw(FourierSeries* fs, size_t num_circles) {
    num_circles = (num_circles < fs->num_circles) ? num_circles : fs->num_circles;
    Complex prev = fs->circles[0].c;
    // draw every coefficient as vector centered on the result of the series so far.
    for(int i = 1; i < num_circles; ++i) {
        Complex curr = complex_add(fs->circles[i].c, prev);
        DrawLineEx(complex_to_vec(prev),
                complex_to_vec(curr),
                VECTOR_THICKNESS,
                VECTOR_COLOR);
        DrawCircleLines(prev.real, prev.img, complex_magnitude(fs->circles[i].c), CIRCLE_COLOR);
        prev = curr;
    }

    // prev will hold the final result (and approximation of the function the series
    // is supposed to approximate).
    dllist_insert_front(fs->last_results, prev);
    if(fs->last_results->size > CIRCLE_TRACE_LENGTH) {
        dllist_pop_back(fs->last_results);
    }

    // draw past results
    DLLNode* point = fs->last_results->head;
    float thickness = CIRCLE_TRACE_THICKNESS;
    // fade out effect
    float thickness_decrement = CIRCLE_TRACE_THICKNESS / fs->last_results->size;
    while(point != NULL && point->succ != NULL) {
        DrawLineEx(complex_to_vec(point->c), 
                complex_to_vec(point->succ->c),
                thickness,
                CIRCLE_TRACE_COLOR);
        point = point->succ;
        thickness -= thickness_decrement;
    }
}
