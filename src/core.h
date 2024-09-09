#ifndef CORE_H
#define CORE_H

#include "src/complex.h"
#include "src/darray.h"
#include <raylib.h>

double* core_segment_lengths(DArray* trace);
Complex* core_lerp_trace(DArray* trace, double dt, size_t* num_points); 
Complex core_numeric_integration(Complex* samples, double dt, size_t num_samples);
/**
 * Uses numerical integration to compute num_coeffs coefficients from
 * -n/2 to n/2. in the fourier series of the curve given by the points in trace.
 *
 * dt determines the step size in the numerical integration.
 */
Complex* core_fourier_coeffs(DArray* trace, double dt, size_t num_coeffs);

#endif
