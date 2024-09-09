#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <raylib.h>


bool utils_almost_equal(double a, double b);
bool utils_same_vector(Vector2 v1, Vector2 v2);
Vector2 utils_sum_vector(Vector2 v1, Vector2 v2);
Vector2 utils_subtract_vector(Vector2 v1, Vector2 v2);
Vector2 utils_scale_vector(Vector2 v, float scalar);
Vector2 utils_lerp_vector(Vector2 begin, Vector2 end, float t);

#endif
