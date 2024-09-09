#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 512 
typedef struct DArray {
    Vector2* points;
    size_t size;
    size_t capacity;
} DArray;

DArray* darray_create(void);
void darray_insert(DArray* darray, Vector2 p);
void darray_free(DArray* darray);
void darray_clear(DArray* darray);


#endif
