#ifndef CIRCLE_H
#define CIRCLE_H

#include "complex.h"
#include "darray.h"
#include "dllist.h"

/**
 * One circle drawn on screen, c is the fourier coeffiecient
 * for the respective frequency freq.
 */
typedef struct Circle {
    Complex c;
    double freq; 
} Circle;

/**
 * All coefficients for the given function/trace on screen.
 *
 * Last _results holds past evaluations of the fourier transform.
 */
typedef struct FourierSeries {
    struct Circle* circles;
    size_t num_circles;
    DLList* last_results;
} FourierSeries;


/**
 * Computes the coefficients of the fourier series given the sample points of trace 
 * using num_coeffs coefficients and a time step of dt (assumend parametrized 
 * from t = 0 to t = 1).
 */
void circle_create(DArray* trace, size_t num_coeffs, double dt, FourierSeries* fs);

/**
 * Frees and reset memory of the given FourierSeries struct.
 */
void circle_free(FourierSeries* fs);

/**
 * Evaluates the next result of the fourier series after the next time step
 */
void circle_step(FourierSeries* fs, double angle_update);

/**
 * Draws all the circles and past evaluations of the series on screen.
 */
void circle_draw(FourierSeries* fs, size_t num_coeffs);

#endif
