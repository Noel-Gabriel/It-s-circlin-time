#include "src/circle.h"
#include "src/darray.h"
#include "src/core.h"

#include <stdio.h>
#include <math.h>
#include <raylib.h>

#define CIRCLE_COLOR GREEN
#define VECTOR_COLOR RED 
#define VECTOR_THICKNESS 2.0f
#define CIRCLE_TRACE_LENGTH 70
#define CIRCLE_TRACE_COLOR YELLOW
#define CIRCLE_TRACE_THICKNESS 10.0f

void circle_free(FourierTransform* ft) {
    if(ft == NULL || ft->circles == NULL) { return; }

    free(ft->circles);
    ft->circles = NULL;
    ft->num_circles = 0;
    dllist_free(ft->last_results);
    ft->last_results = NULL;
}

void circle_create(DArray* trace, size_t num_coeffs, double dt, FourierTransform* ft) {
    circle_free(ft);

    Complex* coefficients = core_fourier_coeffs(trace, dt, num_coeffs);

    ft->circles = malloc(num_coeffs * sizeof(Circle));
    ft->num_circles = num_coeffs;
    ft->last_results = dllist_create();

    for(int i = 0; i < num_coeffs; ++i) {
        int n = (i % 2 == 0)? -1 * i / 2 : (i + 1) / 2;
        Circle c = {coefficients[i], (n == 0)? 0 : (n * 2 * PI)};
        ft->circles[i] = c;
    }
    free(coefficients);
}

void circle_step(FourierTransform* ft, double angle_update) {
    if(ft == NULL || ft->circles == NULL) { return; }

    for(int i = 0; i < ft->num_circles; ++i) {
        double freq = ft->circles[i].freq * angle_update;
        Complex e = {cos(freq), sin(freq)};
        ft->circles[i].c = complex_mult(ft->circles[i].c, e);
    }
}

void circle_draw(FourierTransform* ft, size_t num_circles) {
    num_circles = (num_circles < ft->num_circles) ? num_circles : ft->num_circles;
    Complex prev = ft->circles[0].c;
    for(int i = 1; i < num_circles; ++i) {
        Complex curr = complex_add(ft->circles[i].c, prev);
        DrawLineEx(complex_to_vec(prev),
                complex_to_vec(curr),
                VECTOR_THICKNESS,
                VECTOR_COLOR);
        DrawCircleLines(prev.real, prev.img, complex_magnitude(ft->circles[i].c), CIRCLE_COLOR);
        prev = curr;
    }

    dllist_insert_front(ft->last_results, prev);
    if(ft->last_results->size > CIRCLE_TRACE_LENGTH) {
        dllist_pop_back(ft->last_results);
    }

    DLLNode* point = ft->last_results->head;
    float thickness = CIRCLE_TRACE_THICKNESS;
    float thickness_decrement = CIRCLE_TRACE_THICKNESS / ft->last_results->size;
    while(point != NULL && point->succ != NULL) {
        DrawLineEx(complex_to_vec(point->c), 
                complex_to_vec(point->succ->c),
                thickness,
                CIRCLE_TRACE_COLOR);
        point = point->succ;
        thickness -= thickness_decrement;
    }
}
