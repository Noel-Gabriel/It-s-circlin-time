#define TEST

#ifdef TEST
    #include "test/test.h"
#endif

#include "src/darray.h"
#include "src/utils.h"

#include <stdio.h>
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void draw(DArray* trace) {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        if(trace->size == 0 || !utils_same_vector(trace->points[trace->size-1], mouse_pos)) {
            darray_insert(trace, mouse_pos);
        }
        for(size_t i = 1; i < trace->size; ++i) {
            DrawLineV(trace->points[i-1], trace->points[i], BLACK);
        }
        printf("#Points:\t%zu\n", trace->size);
    } else {
        darray_clear(trace);
    } 
}

int main(void) {
#ifdef TEST
    test_darray();
    test_utils();
#endif

    DArray* trace = darray_create();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circlin' Time");
    
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLUE);
            draw(trace);
        EndDrawing();
    }
    CloseWindow();
    darray_free(trace);
    return 0;
}
