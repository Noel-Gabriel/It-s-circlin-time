#ifndef CIRCLE_H
#define CIRCLE_H

#include "complex.h"
#include <stdlib.h>

typedef struct Circle {
    Complex c;
    double freq; 
} Circle;

void circle_step(Circle* circles, size_t num_circles, double dt);
void circle_draw(Circle* circles, size_t num_circles);

#endif
