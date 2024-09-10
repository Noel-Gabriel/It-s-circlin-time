#define TEST

#ifdef TEST
    #include "test/test.h"
#endif

#include "src/darray.h"
#include "src/utils.h"
#include "src/circle.h"

#define RAYGUI_IMPLEMENTATION
#include "extern/raygui.h"

#include <stdio.h>
#include <raylib.h>

// Raylib & GUI 
#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 600
#define CANVAS_WIDTH (WINDOW_WIDTH - 400)
#define CANVAS_HEIGHT WINDOW_HEIGHT
#define SIDEBAR_WIDTH (WINDOW_WIDTH - CANVAS_WIDTH)

#define CANVAS_COLOR BLACK
#define CANVAS_BORDER_THICKNESS 3.0f
#define CANVAS_BORDER_COLOR WHITE
#define BG_COLOR (Color){0, 3, 75, 255}
#define TRACE_COLOR WHITE 
#define TRACE_LINE_THICKNESS 3.0f
#define FPS 60

// Fourier Transform and update
#define DT 0.0001 
#define NUM_COEFFS 300 
#define MAX_SECONDS_PER_LOOP 10



void read_input(DArray* trace, FourierTransform* ft, Rectangle* canvas) {
    Vector2 mouse_pos = GetMousePosition();
    if(!CheckCollisionPointRec(mouse_pos, *canvas)) { return; }
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        circle_free(ft);
        darray_clear(trace);
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        if(trace->size == 0 
                || !utils_vector_equal(trace->points[trace->size-1], mouse_pos)) {
            darray_insert(trace, mouse_pos);
        } 
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        darray_insert(trace, trace->points[0]);
        if(trace->size < 3) {
            darray_clear(trace);
            return;
        }
        circle_create(trace, NUM_COEFFS, DT, ft);
    } 
}

void draw_trace(DArray* trace) {
    for(size_t i = 1; i < trace->size; ++i) {
        DrawLineEx(trace->points[i-1], 
                trace->points[i], 
                TRACE_LINE_THICKNESS, 
                TRACE_COLOR);
    }
}


int main(void) {

#ifdef TEST
    run_test();
#endif

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circlin' Time");

    DArray* trace = darray_create();

    /************************ DRAW AREA ******************/
    Rectangle canvas = {0, 0, CANVAS_WIDTH, CANVAS_HEIGHT}; 
    /*****************************************************/

    /**************************** NUMBER OF COEFFICIENTS SETTINGS *********************/
    float num_circles = 1.0;

    Rectangle num_circles_slider = {CANVAS_WIDTH + SIDEBAR_WIDTH/4.0f, 
        100.0f, 
        SIDEBAR_WIDTH/2.0f, 
        30.0f};

    Rectangle num_circles_text = {num_circles_slider.x, num_circles_slider.y - 40.0f, 
        num_circles_slider.width, 40.0f};
    /**********************************************************************************/


    /**************************** SPEED SETTINGS **************************************/
    float time_per_loop = 1.0;

    Rectangle time_slider = {CANVAS_WIDTH + SIDEBAR_WIDTH/4.0f, 
        300.0f, 
        SIDEBAR_WIDTH/2.0f, 
        30.0f};

    Rectangle time_text = {time_slider.x, time_slider.y - 40.0f, 
        time_slider.width, 40.0f};
    /**********************************************************************************/

    struct FourierTransform ft;
    ft.circles = NULL;
    ft.num_circles = 0;


    SetTargetFPS(FPS);
    while(!WindowShouldClose()) {

        read_input(trace, &ft, &canvas);

        BeginDrawing();
            
            // Background and drawing area
            ClearBackground(BG_COLOR);
            DrawRectangleRec(canvas, CANVAS_COLOR);
            DrawRectangleLinesEx(canvas, CANVAS_BORDER_THICKNESS, CANVAS_BORDER_COLOR); 

            // Sidebar

            // Coefficients settings
            GuiDrawText("Number of Coefficients", num_circles_text, TEXT_ALIGN_CENTER, WHITE); 
            GuiSlider(num_circles_slider, "1", "299",  &num_circles, 1, NUM_COEFFS-1);

            // Speed settings
            GuiDrawText("Seconds per Loop", time_text, TEXT_ALIGN_CENTER, WHITE); 
            GuiSlider(time_slider, "1", "10",  &time_per_loop, 1, MAX_SECONDS_PER_LOOP);

            // User drawing and Fourier transformation
            draw_trace(trace);
            if(ft.circles != NULL) {
                circle_draw(&ft, (int)num_circles+1);
                circle_step(&ft, (1.0 / (FPS * time_per_loop)));
;
            }

        EndDrawing();
    }

    CloseWindow();
    darray_free(trace);
    circle_free(&ft);

    return 0;
}
