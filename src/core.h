#ifndef CORE_H
#define CORE_H

#include "src/complex.h"
#include "src/darray.h"
#include <raylib.h>

double* core_segment_lengths(DArray* trace);
Complex* core_lerp_trace(DArray* trace, double dt, size_t* num_points); 
//Complex core_num_integrate(DArray* trace, double dt, int coeff);

#endif
