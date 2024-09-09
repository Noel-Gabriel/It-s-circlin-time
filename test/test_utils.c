#include "src/utils.h"
#include <raylib.h>
#include <assert.h>


void test_same_vector(void);
void test_sum_vector(void);
void test_subtract_vector(void);
void test_scale_vector(void);
void test_lerp_vector(void);

void test_utils() {
    test_same_vector();
    test_sum_vector();
    test_subtract_vector();
    test_scale_vector();
    test_lerp_vector();
}

void test_same_vector(void) {
    Vector2 v1 = {3.2, 5};
    Vector2 v2 = {3.2, 5};
    Vector2 v3 = {-3.2, 5};
    Vector2 v4 = {3.1999997, 4.9999998};
    Vector2 v5 = {7, 9}; 
    
    assert(utils_same_vector(v1, v2));
    assert(utils_same_vector(v2, v4));
    assert(!utils_same_vector(v2, v3));
    assert(!utils_same_vector(v3, v4));
    assert(!utils_same_vector(v4, v5));
}

void test_sum_vector(void) {
    Vector2 v1 = {1, 5};
    Vector2 v2 = {5.5, 6.2};
    Vector2 v3 = {-3, -1.3};

    Vector2 v12 = utils_sum_vector(v1, v2);
    Vector2 v13 = utils_sum_vector(v1, v3);
    Vector2 v23 = utils_sum_vector(v2, v3);

    Vector2 v12expected = {1 + 5.5, 5 + 6.2};
    Vector2 v13expected = {1 - 3, 5 - 1.3};
    Vector2 v23expected = {5.5 - 3, 6.2 - 1.3};

    assert(utils_same_vector(v12, v12expected));
    assert(utils_same_vector(v13, v13expected));
    assert(utils_same_vector(v23, v23expected));
}

void test_subtract_vector(void) { 
    Vector2 v1 = {1, 5};
    Vector2 v2 = {5.5, 6.2};
    Vector2 v3 = {-3, -1.3};

    Vector2 v12 = utils_subtract_vector(v1, v2);
    Vector2 v13 = utils_subtract_vector(v1, v3);
    Vector2 v23 = utils_subtract_vector(v2, v3);

    Vector2 v12expected = {1 - 5.5, 5 - 6.2};
    Vector2 v13expected = {1 + 3, 5 + 1.3};
    Vector2 v23expected = {5.5 + 3, 6.2 + 1.3};

    assert(utils_same_vector(v12, v12expected));
    assert(utils_same_vector(v13, v13expected));
    assert(utils_same_vector(v23, v23expected));

}

void test_scale_vector(void) {
    Vector2 v1 = {5.5, 7};
    float s1 = 2.0f;
    float s2 = 1.5f;

    Vector2 v1s1 = utils_scale_vector(v1, s1);
    Vector2 v1s1expected = {5.5f * s1, 7 * s1};
    assert(utils_same_vector(v1s1, v1s1expected));

    Vector2 v1s1s2 = utils_scale_vector(v1s1, s2);
    Vector2 v1s1s2expected = {5.5f * s1 * s2, 7 * s1 * s2};
    assert(utils_same_vector(v1s1s2, v1s1s2expected));
}

void test_lerp_vector(void) {
    Vector2 begin = {1, 2};
    Vector2 end = {3, 10};

    Vector2 lerp1 = utils_lerp_vector(begin, end, 0.5);
    Vector2 lerp1expected = {2, 6};
    assert(utils_same_vector(lerp1, lerp1expected));

    Vector2 lerp2 = utils_lerp_vector(begin, lerp1expected, 0.2);
    Vector2 lerp2expected = {1 + 0.2 * (2 - 1), 2 + 0.2 * (6 - 2)};
    assert(utils_same_vector(lerp2, lerp2expected));
}
