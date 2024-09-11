#define TEST

#ifdef TEST
    #include "test/test.h"
#endif

#include "src/darray.h"
#include "src/utils.h"
#include "src/circle.h"

// Sliders
#define RAYGUI_IMPLEMENTATION
#include "extern/raygui.h"

#include <stdio.h>
#include <raylib.h>

/**** Raylib & GUI ****/

// Window
#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 600
#define BG_COLOR (Color){0, 3, 75, 255}
#define FPS 60

// Canvas (drawing area)
#define CANVAS_WIDTH (WINDOW_WIDTH - 400)
#define CANVAS_HEIGHT WINDOW_HEIGHT
#define CANVAS_COLOR BLACK
#define CANVAS_BORDER_THICKNESS 3.0f
#define CANVAS_BORDER_COLOR WHITE


// Sidebar (sliders)
#define SIDEBAR_WIDTH (WINDOW_WIDTH - CANVAS_WIDTH)

// User drawing
#define TRACE_COLOR WHITE 
#define TRACE_LINE_THICKNESS 3.0f

// Fourier Transform and update
#define DT 0.0001        // time step through drawn curve from t = 0 to t = 1 
#define NUM_COEFFS 300 
#define MAX_SECONDS_PER_LOOP 10



void read_input(DArray* trace, FourierSeries* fs, bool* is_drawing) {
    Vector2 mouse_pos = GetMousePosition();
    // Reset when new drawing
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        circle_free(fs);
        darray_clear(trace);
        *is_drawing = true;
    }
    // save traced curve when holding mouse button
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && *is_drawing) {
        if(trace->size == 0 
                || !utils_vector_equal(trace->points[trace->size-1], mouse_pos)) {
            darray_insert(trace, mouse_pos);
        } 
    // done drawing
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && *is_drawing){
        *is_drawing = false;
        // close loop
        darray_insert(trace, trace->points[0]);
        // need at least (different) 2 points
        if(trace->size < 3) {
            darray_clear(trace);
            return;
        }
        circle_create(trace, NUM_COEFFS, DT, fs);
    } 
}

// draws user trace
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

    struct FourierSeries fs;
    fs.circles = NULL;
    fs.num_circles = 0;

    // to fix a bug where read_input would not reset if a user
    // enters the canvas while holding down the mouse button
    bool is_drawing = false;

    SetTargetFPS(FPS);
    while(!WindowShouldClose()) {
  
        // Read input
        Vector2 mouse_pos = GetMousePosition();
        if(CheckCollisionPointRec(mouse_pos, canvas)) {
            read_input(trace, &fs, &is_drawing);
        }

        BeginDrawing();
            
            // Background and drawing area
            ClearBackground(BG_COLOR);
            DrawRectangleRec(canvas, CANVAS_COLOR);
            DrawRectangleLinesEx(canvas, CANVAS_BORDER_THICKNESS, CANVAS_BORDER_COLOR); 

            /*** Sidebar ***/

            // Coefficients settings
            GuiDrawText("Number of Coefficients", num_circles_text, TEXT_ALIGN_CENTER, WHITE); 
            GuiSlider(num_circles_slider, "1", "299",  &num_circles, 1, NUM_COEFFS-1);

            // Speed settings
            GuiDrawText("Seconds per Loop", time_text, TEXT_ALIGN_CENTER, WHITE); 
            GuiSlider(time_slider, "1", "10",  &time_per_loop, 1, MAX_SECONDS_PER_LOOP);

            // User drawing and FourierSeries evaluation
            draw_trace(trace);
            if(fs.circles != NULL) {
                circle_draw(&fs, (int)num_circles+1);
                circle_step(&fs, (1.0 / (FPS * time_per_loop)));
;
            }

        EndDrawing();
    }

    CloseWindow();
    darray_free(trace);
    circle_free(&fs);

    return 0;
}
