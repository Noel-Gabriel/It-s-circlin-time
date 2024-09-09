#include "test/test.h"
#include "src/darray.h"
#include <malloc/malloc.h>

#include <assert.h>

void test_creation(void);
void test_insertion(void);

void test_darray(void) {
    test_creation();
    test_insertion();
}

void test_creation(void) {
    DArray* darray = darray_create();

    assert(darray != NULL);
    assert(darray->capacity == INITIAL_CAPACITY);
    assert(malloc_size(darray->points) >= INITIAL_CAPACITY);
    assert(darray->size == 0);

    darray_free(darray);
}

void test_insertion(void) {
    DArray* darray = darray_create();

    struct Vector2 v1 = {1.2, 3};
    struct Vector2 v2 = {7, 19.8};

    darray_insert(darray, v1);
    assert(darray->size == 1);

    darray_insert(darray, v2);
    assert(darray->size == 2);

    assert(darray->points[0].x == v1.x && darray->points[0].y == v1.y);
    assert(darray->points[1].x == v2.x && darray->points[1].y == v2.y);
    
    assert(darray->size == 2);
    assert(darray->capacity == INITIAL_CAPACITY);

    for(int i = 0; i < INITIAL_CAPACITY; ++i) {
        struct Vector2 zero = {0.0, 0.0};
        darray_insert(darray, zero);
    }

    assert(darray->size == INITIAL_CAPACITY + 2);
    assert(darray->capacity == INITIAL_CAPACITY * 2);
    assert(malloc_size(darray->points) >= darray->capacity);

    darray_clear(darray);

    assert(darray->size == 0);

    darray_free(darray);
}

