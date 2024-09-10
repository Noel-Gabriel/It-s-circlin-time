#include "src/complex.h"
#include "src/utils.h" 

#include <math.h>
#include <stdio.h>
#include <raylib.h>


bool complex_equal(Complex c1, Complex c2) {
    return utils_almost_equal(c1.real, c2.real) && utils_almost_equal(c1.img, c2.img); 
}

Complex complex_from_vec(Vector2 v) {
    Complex c = {v.x, v.y};
    return c;
}

Vector2 complex_to_vec(Complex c) {
    Vector2 v = {c.real, c.img};
    return v;
}

Complex complex_add(Complex c1, Complex c2) {
    Complex sum = {c1.real + c2.real, c1.img + c2.img};
    return sum;
}

Complex complex_sub(Complex c1, Complex c2) {
    Complex diff = {c1.real - c2.real, c1.img - c2.img};
    return diff;
}

Complex complex_mult(Complex c1, Complex c2) {
    Complex prod = {c1.real * c2.real - c1.img * c2.img,
        c1.real * c2.img + c1.img * c2.real};
    return prod;
}

Complex complex_scale(Complex c, double scalar) {
    Complex scaled = {c.real * scalar, c.img * scalar};
    return scaled;
}

double complex_magnitude(Complex c) {
    return sqrt(c.real * c.real + c.img * c.img); 
}

void complex_print(Complex c) {
    printf("(%f + %fi)", c.real, c.img);
}


