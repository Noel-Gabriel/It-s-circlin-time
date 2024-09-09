#include "src/utils.h"
#include <raylib.h>
#include <assert.h>


void test_vector_equal(void);
void test_vector_add(void);
void test_vector_sub(void);
void test_vector_scale(void);
void test_vector_lerp(void);

void test_utils() {
    test_vector_equal();
    test_vector_add();
    test_vector_sub();
    test_vector_scale();
    test_vector_lerp();
}

void test_vector_equal(void) {
    Vector2 v1 = {3.2, 5};
    Vector2 v2 = {3.2, 5};
    Vector2 v3 = {-3.2, 5};
    Vector2 v4 = {3.1999997, 4.9999998};
    Vector2 v5 = {7, 9}; 
    
    assert(utils_vector_equal(v1, v2));
    assert(utils_vector_equal(v2, v4));
    assert(!utils_vector_equal(v2, v3));
    assert(!utils_vector_equal(v3, v4));
    assert(!utils_vector_equal(v4, v5));
}

void test_vector_add(void) {
    Vector2 v1 = {1, 5};
    Vector2 v2 = {5.5, 6.2};
    Vector2 v3 = {-3, -1.3};

    Vector2 v12 = utils_vector_add(v1, v2);
    Vector2 v13 = utils_vector_add(v1, v3);
    Vector2 v23 = utils_vector_add(v2, v3);

    Vector2 v12expected = {1 + 5.5, 5 + 6.2};
    Vector2 v13expected = {1 - 3, 5 - 1.3};
    Vector2 v23expected = {5.5 - 3, 6.2 - 1.3};

    assert(utils_vector_equal(v12, v12expected));
    assert(utils_vector_equal(v13, v13expected));
    assert(utils_vector_equal(v23, v23expected));
}

void test_vector_sub(void) { 
    Vector2 v1 = {1, 5};
    Vector2 v2 = {5.5, 6.2};
    Vector2 v3 = {-3, -1.3};

    Vector2 v12 = utils_vector_sub(v1, v2);
    Vector2 v13 = utils_vector_sub(v1, v3);
    Vector2 v23 = utils_vector_sub(v2, v3);

    Vector2 v12expected = {1 - 5.5, 5 - 6.2};
    Vector2 v13expected = {1 + 3, 5 + 1.3};
    Vector2 v23expected = {5.5 + 3, 6.2 + 1.3};

    assert(utils_vector_equal(v12, v12expected));
    assert(utils_vector_equal(v13, v13expected));
    assert(utils_vector_equal(v23, v23expected));

}

void test_vector_scale(void) {
    Vector2 v1 = {5.5, 7};
    float s1 = 2.0f;
    float s2 = 1.5f;

    Vector2 v1s1 = utils_vector_scale(v1, s1);
    Vector2 v1s1expected = {5.5f * s1, 7 * s1};
    assert(utils_vector_equal(v1s1, v1s1expected));

    Vector2 v1s1s2 = utils_vector_scale(v1s1, s2);
    Vector2 v1s1s2expected = {5.5f * s1 * s2, 7 * s1 * s2};
    assert(utils_vector_equal(v1s1s2, v1s1s2expected));
}

void test_vector_lerp(void) {
    Vector2 begin = {1, 2};
    Vector2 end = {3, 10};

    Vector2 lerp1 = utils_vector_lerp(begin, end, 0.5);
    Vector2 lerp1expected = {2, 6};
    assert(utils_vector_equal(lerp1, lerp1expected));

    Vector2 lerp2 = utils_vector_lerp(begin, lerp1expected, 0.2);
    Vector2 lerp2expected = {1 + 0.2 * (2 - 1), 2 + 0.2 * (6 - 2)};
    assert(utils_vector_equal(lerp2, lerp2expected));
}
