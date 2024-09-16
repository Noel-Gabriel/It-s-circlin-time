#define TEST

#ifdef TEST
    #include "test/test.h"
#endif

#include "src/gui.h"

#include <raylib.h>

int main(void) {

#ifdef TEST
    run_test();
#endif

    gui_init();

    while(!WindowShouldClose()) {
        // Read input
        gui_read_input(); 
        gui_draw(); 
    }

    CloseWindow();
    gui_free();
    return 0;
}
