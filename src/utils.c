#include "src/utils.h"
#include <stdbool.h>
#include <raylib.h>

#define FLOAT_EPSILON 0.000001f

bool utils_almost_equal(float a, float b) {
    return (((a < b)? b - a : a - b) <= FLOAT_EPSILON);
}

bool utils_same_vector(Vector2 v1, Vector2 v2) {
    return utils_almost_equal(v1.x, v2.x) && utils_almost_equal(v1.y, v2.y);
}

Vector2 utils_sum_vector(Vector2 v1, Vector2 v2) {
    Vector2 sum = {v1.x + v2.x, v1.y + v2.y};
    return sum;
}

Vector2 utils_subtract_vector(Vector2 v1, Vector2 v2) {
    Vector2 diff = {v1.x - v2.x, v1.y - v2.y};
    return diff;
}

Vector2 utils_scale_vector(Vector2 v, float scalar) {
    Vector2 scale = {v.x * scalar, v.y * scalar};
    return scale;
}

Vector2 utils_lerp_vector(Vector2 begin, Vector2 end, float t) {
    Vector2 interp = {begin.x + t * (end.x - begin.x), begin.y + t * (end.y - begin.y)};
    return interp;
}
