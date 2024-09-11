#ifndef COMPLEX_H
#define COMPLEX_H

#include <raylib.h>

/** Simple complex number */
typedef struct Complex {
    double real;
    double img;
} Complex;

/**
 * True if the complex numbers are (almost) equal.
 */
bool complex_equal(Complex c1, Complex c2);

/**
 * Returns the vector as Complex (x = real, y = img).
 */
Complex complex_from_vec(Vector2 v);

/**
 * Returns the complex number as Vector2 (real = x, img = y).
 */
Vector2 complex_to_vec(Complex c);

/**
 * Adds to complex numbers.
 */
Complex complex_add(Complex c1, Complex c2);

/**
 * Subtracts the second complex number from the first.
 */
Complex complex_sub(Complex c1, Complex c2);

/**
 * Multiplies the given complex numbers.
 */
Complex complex_mult(Complex c1, Complex c2);

/**
 * Scales the given complex number.
 */
Complex complex_scale(Complex c, double scalar);

/**
 * Returns the length of the complex number as vector (Euclidean distance).
 */
double complex_magnitude(Complex c);

/**
 * Prints the complex number on stdout.
 */
void complex_print(Complex c);

#endif
