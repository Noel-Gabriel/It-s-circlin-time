#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

// Initial capacity of the dynamic array
#define INITIAL_CAPACITY 512 

/** Simple dynamic array */
typedef struct DArray {
    Vector2* points;
    size_t size;
    size_t capacity;
} DArray;

/** 
 * Creates and allocates the memory for a new array.
 */
DArray* darray_create(void);

/**
 * Inserts the element in the array.
 */
void darray_insert(DArray* darray, Vector2 p);

/**
 * Frees all allocated memory of the array.
 */
void darray_free(DArray* darray);

/**
 * Resets the size of the array to 0.
 */
void darray_clear(DArray* darray);


#endif
