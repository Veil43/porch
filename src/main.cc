#include "window.hpp"

int main(int argc, char** argv) {
    Window main_window = Window{800.0, 1.0, "Porch Renderer"};
    main_window.launch_window_loop();
    
    return 0;
}