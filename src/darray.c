#include "stdio.h"
#include "darray.h"

DArray* darray_create(void) {
    DArray* darray = malloc(sizeof *darray);
    darray->points = malloc(INITIAL_CAPACITY * sizeof *(darray->points));
    darray->size = 0;
    darray->capacity = INITIAL_CAPACITY;
    return darray;
}

void darray_insert(DArray* darray, Vector2 p) {
    if(darray == NULL) { return; }
    if(darray->size == darray->capacity) {
        Vector2* temp = realloc(darray->points, darray->capacity * 2 * sizeof(Vector2));
        if(temp == NULL) {
            printf("DArray Realloc failed.");
            return;
        }
        darray->points = temp;
        darray->capacity *= 2;
    }
    darray->points[darray->size++] = p;
}

void darray_free(DArray* darray) {
    if(darray == NULL) { return; }
    free(darray->points);
    free(darray);
}

void darray_clear(DArray* darray) {
    if(darray == NULL) { return; }
    darray->size = 0;
}
