#include "src/gui.h"

#define RAYGUI_IMPLEMENTATION
#include "extern/raygui.h"

#include "src/circle.h"
#include "src/darray.h"
#include "src/utils.h"
#include "src/complex.h"
#include "src/svgloader.h"

/**** Raylib & GUI ****/

// Window
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define BG_COLOR (Color){0, 3, 75, 255}
#define FPS 120

// Canvas (drawing area)
#define CANVAS_WIDTH (WINDOW_WIDTH - 300)
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
#define MAX_SECONDS_PER_LOOP 60

// SVG Loading
#define SVG_SAMPLE_DT 0.01
#define MAX_PATH_SIZE 200

// components helper functions
void init_coeffs_slider(void);
void init_time_slider(void);
void init_svg_input(void);
void init_follow_button(void);

// camera setting helper functions
void reset_camera(void);
void follow_camera(void);

// drawing helper functions
void draw_coeffs_slider(void);
void draw_speed_slider(void);
void draw_svg_input(void);
void draw_follow_button(void);
void draw_trace(void);
void draw_circles(void);

// Current state of the Gui
typedef struct Gui {
    // State of the gui
    DArray* trace;
    FourierSeries fs;
    Rectangle canvas;
    Camera2D camera;
    bool is_drawing;
    bool is_following;
    float num_circles;
    float time_per_loop;
    char* path;

    // components
    // coeffs slider
    Rectangle coeffs_text_bounds; 
    Rectangle coeffs_slider_bounds;

    // speed slider
    Rectangle time_text_bounds;
    Rectangle time_slider_bounds;

    // svg input box
    Rectangle svg_text_bounds;
    Rectangle svg_input_bounds;

    // follow button
    Rectangle follow_button_bounds;
} Gui;

static Gui state;

void gui_init(void) {
    // initialize gui variables
    state.trace = darray_create();

    state.fs.circles = NULL;
    state.fs.num_circles = 0;
    state.fs.last_results = NULL;

    state.canvas = (Rectangle){0, 0, CANVAS_WIDTH, CANVAS_HEIGHT}; 

    state.camera = (Camera2D){
        (Vector2){WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f},
        (Vector2){WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f},
        0.0,
        1.0
    };

    state.is_drawing = false;
    state.is_following = false;

    // initialize components
    init_coeffs_slider();
    init_time_slider();
    init_svg_input();
    init_follow_button();

    // initialize window
    SetTargetFPS(FPS);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circlin' Time");
}


/********************** Gui Components Initialization **************************/

void init_coeffs_slider() {
    state.num_circles = 1.0;

    state.coeffs_slider_bounds = (Rectangle){CANVAS_WIDTH + SIDEBAR_WIDTH/4.0f, 
        80.0f, 
        SIDEBAR_WIDTH/2.0f, 
        30.0f};

    state.coeffs_text_bounds = (Rectangle){state.coeffs_slider_bounds.x, 
        state.coeffs_slider_bounds.y - 40.0f, 
        state.coeffs_slider_bounds.width, 40.0f};
}

void init_time_slider() {
    state.time_per_loop = 1.0;

    state.time_slider_bounds = (Rectangle){CANVAS_WIDTH + SIDEBAR_WIDTH/4.0f, 
        180.0f, 
        SIDEBAR_WIDTH/2.0f, 
        30.0f};

    state.time_text_bounds = (Rectangle){state.time_slider_bounds.x, 
        state.time_slider_bounds.y - 40.0f, 
        state.time_slider_bounds.width, 40.0f};
}

void init_svg_input(void) {
    state.path = malloc(MAX_PATH_SIZE * sizeof(char));
    if(state.path == NULL) { 
        exit(1);
    }

    state.svg_input_bounds = (Rectangle){CANVAS_WIDTH + SIDEBAR_WIDTH/4.0f, 
        280.0f, 
        SIDEBAR_WIDTH/2.0f, 
        30.0f};

    state.svg_text_bounds = (Rectangle){state.svg_text_bounds.x, 
        state.svg_text_bounds.y - 40.0f, 
        state.svg_text_bounds.width, 40.0f};
}

void init_follow_button(void) {
    state.follow_button_bounds = (Rectangle){CANVAS_WIDTH 
        + SIDEBAR_WIDTH/2.0f - SIDEBAR_WIDTH/8.0f, 
        380.0f, 
        SIDEBAR_WIDTH/4.0f, 
        30.0f};
}

/******************************************************************************/

void reset_camera(void) {
    state.is_following = false;
    state.camera.target = (Vector2){WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f};
    state.camera.zoom = 1.0f;
}

void follow_camera(void) {
    if(state.fs.last_results != NULL && state.fs.last_results->size > 0) {
        state.camera.target = complex_to_vec(state.fs.last_results->head->c);
        state.camera.zoom = 5.0f;
    }
}

void gui_read_input() {  
    Vector2 mouse_pos = GetMousePosition();

    // exit zoom
    if(state.is_following && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        reset_camera(); 
        return;
    }

    // Check input on canvas
    if(CheckCollisionPointRec(mouse_pos, state.canvas)) {
        // reset at press
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            circle_free(&state.fs);
            darray_clear(state.trace);
            state.is_drawing = true;
        }

        // draw if hold
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && state.is_drawing) {
            if(state.trace->size == 0 
                || !utils_vector_equal(state.trace->points[state.trace->size-1], mouse_pos)) {
            darray_insert(state.trace, mouse_pos);
            } 
        }

        // create circles at release
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && state.is_drawing){
            state.is_drawing = false;
            // close loop
            darray_insert(state.trace, state.trace->points[0]);
            // need at least (different) 2 points
            if(state.trace->size < 3) {
                darray_clear(state.trace);
                return;
            }
            circle_create(state.trace, NUM_COEFFS, DT, &state.fs);
        } 
    }

    // copy-paste on text input
    if(!state.is_drawing && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))) {
        if(IsKeyPressed(KEY_V)) {
            const char* pasted = GetClipboardText();
            if(pasted != NULL) {
                int pos = 0;
                while(pasted[pos] != '\0' && pos < MAX_PATH_SIZE-1) {
                    state.path[pos] = pasted[pos];
                    ++pos;
                }
                state.path[pos] = '\0';
            }
        }
    }
}

void draw_coeffs_slider(void) {
    GuiDrawText("Number of Coefficients", state.coeffs_text_bounds, TEXT_ALIGN_CENTER, WHITE); 
    GuiSlider(state.coeffs_slider_bounds, "1", "299",  &state.num_circles, 1, NUM_COEFFS-1);
}

void draw_speed_slider(void) {
    GuiDrawText("Seconds per Loop", state.time_text_bounds, TEXT_ALIGN_CENTER, WHITE); 
    GuiSlider(state.time_slider_bounds, "1", "10",  &state.time_per_loop, 1, MAX_SECONDS_PER_LOOP);
}

void draw_svg_input(void) {
    GuiDrawText("Load svg", state.svg_text_bounds, TEXT_ALIGN_CENTER, WHITE);
    if(GuiTextBox(state.svg_input_bounds, state.path, MAX_PATH_SIZE, true)) {
        DArray* svg = svgloader_sample_svg(state.path, SVG_SAMPLE_DT, CANVAS_WIDTH-20.0f, CANVAS_HEIGHT-20.0f);
        if(svg != NULL) {
            darray_free(state.trace);
            circle_free(&state.fs);
            state.trace = svg;
            circle_create(state.trace, NUM_COEFFS, DT, &state.fs);
        }
        state.path[0] = '\0';
    }
}

void draw_follow_button(void) {
    bool pressed = GuiButton(state.follow_button_bounds, "Follow!");
    state.is_following |= pressed;
    if(state.is_following) {
        follow_camera();
    }
}

// draw trace on canvas
void draw_trace(void) {
    for(size_t i = 1; i < state.trace->size; ++i) {
        DrawLineEx(state.trace->points[i-1], 
                state.trace->points[i], 
                TRACE_LINE_THICKNESS, 
                TRACE_COLOR);
    }
}

// draw circles
void draw_circles(void) {
    if(state.fs.circles != NULL) {
        circle_draw(&state.fs, (int)state.num_circles+1);
        circle_step(&state.fs, (1.0 / (FPS * state.time_per_loop)));
    }
}

void gui_draw() {
    BeginDrawing();
           
        BeginMode2D(state.camera);

        // Background and drawing area
        ClearBackground(BG_COLOR);
        DrawRectangleRec(state.canvas, CANVAS_COLOR);
        DrawRectangleLinesEx(state.canvas, CANVAS_BORDER_THICKNESS, CANVAS_BORDER_COLOR); 

        /*** Sidebar ***/
        draw_coeffs_slider();
        draw_speed_slider();
        draw_svg_input();
        draw_follow_button();

        // user trace and circles
        draw_trace();
        draw_circles();
       
        EndMode2D();

    EndDrawing();
}

void gui_free(void) {
    darray_free(state.trace);
    circle_free(&state.fs);
    free(state.path);
}
