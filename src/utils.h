#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <raylib.h>


/**
 * Double comparison using a smaller epsilon.
 */
bool utils_equal(double a, double b);

/**
 * Double comparison using a larger epsilon (used for vector/complex comparison,
 * lots of rounding errors and does not need to be too precise.)
 */
bool utils_almost_equal(double a, double b);

/**
 * Returns true if vectors are close enough.
 */
bool utils_vector_equal(Vector2 v1, Vector2 v2);

/**
 * Adds the two given vectors.
 */
Vector2 utils_vector_add(Vector2 v1, Vector2 v2);

/**
 * Subtracts the second vector from the first one.
 */
Vector2 utils_vector_sub(Vector2 v1, Vector2 v2);

/**
 * Scales the given vector.
 */
Vector2 utils_vector_scale(Vector2 v, float scalar);

/**
 * Linear interpolation between begin and end.
 */
Vector2 utils_vector_lerp(Vector2 begin, Vector2 end, float t);

/**
 * Returns the length of the vector (euclidean distance).
 */
double utils_vector_magnitude(Vector2 v);

#endif
