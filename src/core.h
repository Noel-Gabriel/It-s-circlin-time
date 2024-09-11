#ifndef CORE_H
#define CORE_H

#include "src/complex.h"
#include "src/darray.h"
#include <raylib.h>

/**
 * Computes the length of the arc from the starting point in trace to any
 * other point in trace. 
 *
 * The length of the returned array is always equal to the number of points in
 * trace (size).
 *
 * Thus, for the returned array arr, the last element in arr represents
 * the total arc length of the curve (linearly interpolated)
 * given by the sampled points.
 */
double* core_segment_lengths(DArray* trace);

/**
 * Returns sampled points evenly spaced on the (linearly interpolated) curve
 * given by trace. 
 *
 * dt determines the time step, assuming a parametrization from t = 0 to t = 1.
 */
Complex* core_lerp_trace(DArray* trace, double dt, size_t* num_points); 

/**
 * Numerically integrates the sampled function given by sample, assumed that
 * a time step of dt was used for sampling.
 */
Complex core_numeric_integration(Complex* samples, double dt, size_t num_samples);

/**
 * Uses numerical integration to compute num_coeffs coefficients from
 * -n/2 to n/2. in the fourier series of the curve given by the points in trace.
 *
 * dt determines the step size in the numerical integration/sampling.
 */
Complex* core_fourier_coeffs(DArray* trace, double dt, size_t num_coeffs);

#endif
