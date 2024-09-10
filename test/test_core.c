#include "test/test.h"
#include "src/core.h"
#include "src/darray.h"
#include "src/utils.h"
#include "src/complex.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

void test_core_segment_lengths(void);
void test_core_lerp_trace(void);
void test_core_numeric_integration(void);
void test_core_fourier_coefficients(void);

void test_core(void) {
    test_core_segment_lengths();
    test_core_lerp_trace();
    test_core_numeric_integration();
    test_core_fourier_coefficients();
}

void test_core_segment_lengths(void) {
    DArray* darray = darray_create();
    for(int i = 0; i < 10; ++i) {
        // all length 1 segments
        Vector2 v = {i, 0};
        darray_insert(darray, v);
    }

    double* lengths = core_segment_lengths(darray);
    double arc = 0.0;
    for(int i = 0; i < darray->size; ++i) {
        assert(utils_almost_equal(arc, lengths[i]));
        ++arc;
    }

    free(lengths);

    // 5 x 3 Rectangle
    Vector2 v1 = {0, 0}; 
    Vector2 v2 = {5, 0}; 
    Vector2 v3 = {5, -3};
    Vector2 v4 = {0, -3};

    darray_clear(darray);

    darray_insert(darray, v1);
    darray_insert(darray, v2);
    darray_insert(darray, v3);
    darray_insert(darray, v4);
   
    lengths = core_segment_lengths(darray);

    assert(utils_almost_equal(lengths[0], 0.0));
    assert(utils_almost_equal(lengths[1], 5.0));
    assert(utils_almost_equal(lengths[2], 8.0));
    assert(utils_almost_equal(lengths[3], 13.0));
   
    free(lengths);
    darray_free(darray);
}

void test_core_lerp_trace(void) {
    DArray* trace = darray_create();
    // Line segment from 0 to 1
    Vector2 start = {0, 0};
    Vector2 end = {1, 0};

    darray_insert(trace, start);
    darray_insert(trace, end);

    size_t size = 0;
     // expect {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9} x {0} points
    double dt = 0.1;    
    Complex* points = core_lerp_trace(trace, dt, &size);
    assert(points != NULL);
    assert(size == 10);
    
    for(int i = 0; i < size; ++i) {
        Complex c = {i * dt, 0};
        assert(complex_equal(points[i], c));
    }
    free(points);

    Vector2 end2 = {1, 1};
    darray_insert(trace, end2);

    dt = 0.2;

    points = core_lerp_trace(trace, dt, &size);
    assert(points != NULL);
    assert(size == 5); // (0, 0), (0.4, 0), (0.8, 0), (1, 0.2), (1, 0.6)

    Complex c1 = {0, 0};
    Complex c2 = {0.4, 0};
    Complex c3 = {0.8, 0};
    Complex c4 = {1, 0.2};
    Complex c5 = {1, 0.6};

    assert(complex_equal(c1, points[0]));
    assert(complex_equal(c2, points[1]));
    assert(complex_equal(c3, points[2]));
    assert(complex_equal(c4, points[3]));
    assert(complex_equal(c5, points[4]));
    
    free(points);
    darray_free(trace);
}

void test_core_numeric_integration(void) {
    
    size_t num_samples = 10000000;

    // integral of x^2 from 0 to 2
    int from = 0;
    int to = 2;
    Complex* points = malloc(num_samples * sizeof(Complex));
    double dx = (to - 1.0 * from) / num_samples;
    double x = from;
    for(int i = 0; i < num_samples; ++i) {
        Complex y = {x*x, 0};
        points[i] = y;
        x += dx;
    }

    Complex integral = core_numeric_integration(points, dx, num_samples);
    // (2^3/3) - (0^3/3) = 8 / 3
    Complex expected = {1.0*8/3, 0};
    assert(complex_equal(integral, expected));

    // integral of f(z) = z
    // on C(t) = 3*t-i*t^2 from -1 to 4.
    from = -1;
    to = 4;
    double dt = (to - 1.0 * from) / num_samples;
    double t = from;
    for(int i = 0; i < num_samples; ++i) {
        Complex y = {3*t, - (t*t)};
        points[i] = y;
        t += dt;
    }

    integral = core_numeric_integration(points, dt, num_samples);
    Complex expected2 = {22.5, -21.666667};
    assert(complex_equal(integral, expected2));

    free(points);
}

void test_core_fourier_coefficients(void) {
    // circle with radius 1
    DArray* darray = darray_create();
    int num_samples = 10000;
    double dt = 2 * PI / num_samples;
    double t = 0.0;
    for(int i = 0; i < num_samples; ++i) {
        Vector2 v = {cos(t),  sin(t)};
        darray_insert(darray, v);
        t += dt;
    }
    size_t num_coeffs = 20;
    Complex* coeffs = core_fourier_coeffs(darray, 0.01 , num_coeffs);
    Complex center = {0, 0};
    Complex radius = {1, 0};
    assert(complex_equal(coeffs[0], center));
    assert(complex_equal(coeffs[1], radius));
    for(int i = 2; i < num_coeffs; ++i) {
        assert(complex_equal(coeffs[i], center));
    }
    free(coeffs);
    darray_free(darray);
}
