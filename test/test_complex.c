#include "test/test.h"
#include "src/complex.h"
#include "src/utils.h"

#include <assert.h>
#include <raylib.h>

void test_complex_from_to_vec(void);
void test_complex_add(void);
void test_complex_sub(void);
void test_complex_mult(void);
void test_complex_scale(void);
void test_complex_magnitude(void);

void test_complex(void) {
    test_complex_from_to_vec();
    test_complex_add();
    test_complex_sub();
    test_complex_mult();
    test_complex_scale();
    test_complex_magnitude();
}

void test_complex_from_to_vec(void) {
    Vector2 v = {43.2, -4.3};

    Complex cfromv = complex_from_vec(v);
    Vector2 ctov = complex_to_vec(cfromv);

    assert(cfromv.real == v.x);
    assert(cfromv.img == v.y);
    assert(ctov.x == v.x);
    assert(ctov.y == v.y);
}

void test_complex_add(void) {
    Complex c1 = {43, -12};
    Complex c2 = {17, 12};

    Complex s1 = complex_add(c1, c2);
    Complex s1expected = {43 + 17, -12 + 12};

    Complex c3 = {123.5912311, -45.234952342};
    Complex c4 = {-1234.22323, 42349.019123};
    
    Complex s2 = complex_add(c3, c4);
    Complex s2expected = {123.5912311 + -1234.22323, -45.234952342 + 42349.019123};

    assert(complex_equal(s1, s1expected));
    assert(complex_equal(s2, s2expected));
}

void test_complex_sub(void) {
    Complex c1 = {43, -12};
    Complex c2 = {17, 12};

    Complex s1 = complex_sub(c1, c2);
    Complex s1expected = {43 - 17, (-12) - 12};

    Complex c3 = {123.5912311, -45.234952342};
    Complex c4 = {-1234.22323, 42349.019123};
    
    Complex s2 = complex_sub(c3, c4);
    Complex s2expected = {123.5912311 - (-1234.22323), (-45.234952342) - 42349.019123};

    assert(complex_equal(s1, s1expected));
    assert(complex_equal(s2, s2expected));
}

void test_complex_mult(void) {
    Complex c1 = {1, 1};
    Complex c2 = {0, 1};
    
    Complex m1 = complex_mult(c1, c2);
    Complex m1expected = {-1, 1};

    Complex c3 = {23.2341, 1.432};
    Complex c4 = {14.3245, -423.49};

    Complex m2 = complex_mult(c3, c4);
    Complex m2expected = {939.25454545, -9818.896325};

    assert(complex_equal(m1, m1expected));
    assert(complex_equal(m2, m2expected));
}

void test_complex_scale(void) {
    Complex c1 = {5.2, 4};
    double scalar1 = 0.42;

    Complex s1 = complex_scale(c1, scalar1);
    Complex s1expected = {5.2 * 0.42, 4 * 0.42};

    Complex c2 = {-142.54166, 114.23};
    double scalar2 = -52.52346245;

    Complex s2 = complex_scale(c2, scalar2);
    Complex s2expected = {-142.54166 * scalar2, 114.23 * scalar2};

    assert(complex_equal(s1, s1expected));
    assert(complex_equal(s2, s2expected));
}

void test_complex_magnitude(void) {
    Complex c1 = {3, -4};

    double m1 = complex_magnitude(c1);
    double m1expected = {5.0};

    Complex c2 = {2.543245, 0.1234};

    double m2 = complex_magnitude(c2);
    double m2expected = {2.54623696659};

    assert(utils_almost_equal(m1, m1expected));
    assert(utils_almost_equal(m2, m2expected));
}
