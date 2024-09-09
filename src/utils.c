#include "src/utils.h"
#include <stdbool.h>
#include <raylib.h>

bool utils_same_vector(Vector2 v1, Vector2 v2) {
    return v1.x == v2.x && v1.y == v2.y;
}
