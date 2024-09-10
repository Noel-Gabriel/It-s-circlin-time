#ifndef CIRCLE_H
#define CIRCLE_H

#include "complex.h"
#include "darray.h"
#include "dllist.h"

#include <stdlib.h>

typedef struct Circle {
    Complex c;
    double freq; 
} Circle;

typedef struct FourierTransform {
    struct Circle* circles;
    size_t num_circles;
    DLList* last_results;
} FourierTransform;


void circle_create(DArray* trace, size_t num_coeffs, double dt, FourierTransform* ft);
void circle_free(FourierTransform* ft);
void circle_step(FourierTransform* ft, double angle_update);
void circle_draw(FourierTransform* ft, size_t num_coeffs);

#endif
