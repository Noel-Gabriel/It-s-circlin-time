#define TEST

#ifdef TEST
    #include "test/test.h"
#endif

#include "src/darray.h"
#include "src/utils.h"
#include "src/complex.h"
#include "src/core.h"
#include "src/circle.h"

#include <stdio.h>
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60
#define DT 0.001


Circle* circles = NULL;
size_t num_circles = 0;
double t = 0.0;

void make_circles(DArray* trace) {
    size_t num_coeffs = 300;
    Complex* coefficients = core_fourier_coeffs(trace, DT, num_coeffs);
    circles = malloc(num_coeffs * sizeof(Circle));
    num_circles = num_coeffs;
    for(int i = 0; i < num_coeffs; ++i) {
        int n = (i % 2 == 0)? -1 * i / 2 : (i + 1) / 2;
        Circle c = {coefficients[i], (n == 0)? 0 : n * 2 * PI / (FPS)};
        circles[i] = c;
        complex_print(coefficients[i]);
        printf("\n");
    }
}

void get(DArray* trace) {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && num_circles == 0) {
        Vector2 mouse_pos = GetMousePosition();
        if(trace->size == 0 || !utils_vector_equal(trace->points[trace->size-1], mouse_pos)) {
            darray_insert(trace, mouse_pos);
        } 
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && num_circles == 0){
        darray_insert(trace, trace->points[0]);
        make_circles(trace);
    } 
}

void draw_trace(DArray* trace) {
    for(size_t i = 1; i < trace->size; ++i) {
            DrawLineV(trace->points[i-1], trace->points[i], BLACK);
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
    
    SetTargetFPS(FPS);
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(GRAY);
            get(trace);
            draw_trace(trace);
            circle_draw(circles, num_circles);
            circle_step(circles, num_circles, &t, DT);
        EndDrawing();
    }
    CloseWindow();
    darray_free(trace);
    free(circles);
    return 0;
}
