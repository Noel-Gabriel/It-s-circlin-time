#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <raylib.h>


bool utils_equal(double a, double b);
bool utils_almost_equal(double a, double b);
bool utils_vector_equal(Vector2 v1, Vector2 v2);
Vector2 utils_vector_add(Vector2 v1, Vector2 v2);
Vector2 utils_vector_sub(Vector2 v1, Vector2 v2);
Vector2 utils_vector_scale(Vector2 v, float scalar);
Vector2 utils_vector_lerp(Vector2 begin, Vector2 end, float t);
double utils_vector_magnitude(Vector2 v);

#endif
