#define TEST

#ifdef TEST
    #include "test/test.h"
#endif

#include "src/darray.h"
#include "src/utils.h"
#include "src/complex.h"
#include "src/core.h"

#include <stdio.h>
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void draw_checks(DArray* trace, double dt) {
    size_t size = 0;
    Complex* points = core_lerp_trace(trace, dt, &size);
    for(int i = 0; i < size; ++i) {
        DrawPixel(points[i].real, points[i].img, RED);
    }
    free(points);
}

void draw(DArray* trace) {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        if(trace->size == 0 || !utils_vector_equal(trace->points[trace->size-1], mouse_pos)) {
            darray_insert(trace, mouse_pos);
        }
        for(size_t i = 1; i < trace->size; ++i) {
            DrawLineV(trace->points[i-1], trace->points[i], BLACK);
        }
    } else {
        draw_checks(trace, 0.0001);
    } 
}

int main(void) {
#ifdef TEST
    test_darray();
    test_utils();
    test_complex();
    test_core();
#endif

    DArray* trace = darray_create();
    
    SetTraceLogLevel(LOG_WARNING);
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
