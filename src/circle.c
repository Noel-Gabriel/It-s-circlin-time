#include "circle.h"
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "darray.h"

DArray* points = NULL;

void draw_points(Complex last) {
    if(points == NULL) {
        points = darray_create();
    }
    Vector2 v = {last.real, last.img};
    darray_insert(points, v);
    for(int i = 0; i < points->size; ++i) {
        DrawPixelV(points->points[i], WHITE);
    }
}

#include <stdio.h>
void circle_step(Circle* circles, size_t num_circles,double dt) {
    if(circles == NULL) { return; }
    for(int i = 0; i < num_circles; ++i) {
        Complex e = {cos(circles[i].freq*(dt)), sin(circles[i].freq*(dt))};
        circles[i].c = complex_mult(circles[i].c, e);
    }
}

void circle_draw(Circle* circles, size_t num_circles) {
    if(circles == NULL) { return; }
    Complex prev = circles[0].c;
    for(int i = 1; i < num_circles; ++i) {
        Complex curr = complex_add(circles[i].c, prev);
        DrawLine(prev.real, prev.img, curr.real, curr.img, RED);
        DrawCircleLines(prev.real, prev.img, complex_magnitude(circles[i].c), GREEN);
        prev = curr;
    }
    draw_points(prev);
}
