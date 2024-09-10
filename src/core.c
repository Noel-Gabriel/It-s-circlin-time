#include "src/core.h"
#include "src/complex.h"
#include "src/darray.h"
#include "src/utils.h"
#include "stdio.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/**
 * Computes the length of the arc from the starting point in the DArray to any
 * other point in the DArray.
 *
 * The length of the returned array is always equal to the number of points in
 * the DArray (size).
 *
 * Thus, for the returned array arr, the last element in arr represents
 * the total arc length of the curve (linearly interpolated)
 * given by the points in the DArray.
 */
double* core_segment_lengths(DArray* trace) {
    assert(trace != NULL);
    if(trace->size < 2) { return NULL; }
    double* len = malloc(trace->size * sizeof(double));
    assert(len != NULL);

    len[0] = 0.0;
    for(size_t i = 1; i < trace->size; ++i) {
        Vector2 segment = utils_vector_sub(
                trace->points[i],
                trace->points[i-1]);
        // arc length so far = arc length last point + length of current segment
        len[i] = len[i-1] + utils_vector_magnitude(segment); 
    }
    return len;
}

/**
 * Parameterizes the interpolated curve in DArray and returns the point at every
 * dt time step (paramerization from t = 0 to t = 1).
 *
 * E.g, for dt = 0.1, this function returns (1 / 0.1) = 10 points, 
 * each at equal distance from each other (curve_length / 10) starting with
 * the first point.
 */
Complex* core_lerp_trace(DArray* trace, double dt, size_t* num_points) {
    if(trace == NULL || !(0 < dt && dt <= 1)) { return NULL; }
    double* segment_lengths = core_segment_lengths(trace);
    if(segment_lengths == NULL) { return NULL; }


    // parameterize from t = 0 to t = 1 for whole curve
    double step = segment_lengths[trace->size-1] / (1 / dt);
    *num_points = (size_t)floor(1 / dt);
    Complex* points = malloc(*num_points * sizeof(Complex));
    assert(points != NULL);
    assert(!utils_equal(step, 0.0));
    
 
    double dist = 0.0;
    int segment = 1;
    for(int i = 0; i < *num_points; ++i) {
        while(segment_lengths[segment] < dist) {
            assert(segment < trace->size);
            ++segment;
        }

        double prev = segment_lengths[segment-1];
        double t = (dist - prev) / (segment_lengths[segment] - prev);
        Vector2 point = utils_vector_lerp(
                trace->points[segment-1],  
                trace->points[segment],
                t);
        points[i] = complex_from_vec(point);
        dist += step;
    }
    free(segment_lengths);
    return points;
}

/**
 * Helper function for the numerical integration.
 */
Complex core_numeric_integration(Complex* samples, double dt, size_t num_samples) {
    Complex integral = {0, 0};
    for(int i = 0; i < num_samples; ++i) {
        integral = complex_add(integral, complex_scale(samples[i], dt));
    }
    return integral;
}

/**
 * Uses numerical integration to compute num_coeffs coefficients from
 * -n/2 to n/2. in the fourier series of the curve given by the points in trace.
 *
 * dt determines the step size in the numerical integration.
 */
Complex* core_fourier_coeffs(DArray* trace, double dt, size_t num_coeffs) {
    size_t size = 0;
    Complex* points = core_lerp_trace(trace, dt, &size);
    Complex* points_copy = malloc(size * sizeof(Complex));
    assert(points != NULL);
    assert(points_copy != NULL);

    // copy
    for(int i = 0; i < size; ++i) {
        points_copy[i] = points[i];
    }

    Complex* coefficients = malloc(num_coeffs * sizeof(Complex));
    assert(coefficients != NULL);
    
    for(int i = 0; i < num_coeffs; ++i) {
        int n = (i % 2 == 0)? -1 * i/2 : (i+1)/2;
        double t = 0.0;
        for(int p = 0; p < size; ++p) {
            double w = -1 * n * 2 * PI * t;
            Complex e = {cos(w), sin(w)};
            points_copy[p] = complex_mult(points[p], e);
            t += dt;
        }
        coefficients[i] = core_numeric_integration(points_copy, dt, size);
    }
    free(points_copy);
    free(points);
    return coefficients;
}
