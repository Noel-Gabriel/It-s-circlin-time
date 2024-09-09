#ifndef COMPLEX_H
#define COMPLEX_H

#include <raylib.h>

typedef struct Complex {
    double real;
    double img;
} Complex;

bool complex_equal(Complex c1, Complex c2);
Complex complex_from_vec(Vector2 v);
Complex complex_add(Complex c1, Complex c2);
Complex complex_sub(Complex c1, Complex c2);
Complex complex_mult(Complex c1, Complex c2);
Complex complex_scale(Complex c, double scalar);
double complex_magnitude(Complex c);
void complex_print(Complex c);

#endif
