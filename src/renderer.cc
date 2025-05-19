#include "renderer.hh"
#include "window.hh"

namespace renderer
{
/// NOTE: Find out when thi is initialized
static Window* GLOBAL_s_window_ptr;

DECL_WINDOW_RENDER_CALLBACK(__window_render_callback) {
    static constexpr bool tick = true;
    if (tick) {
        return utils::load_image_from_file("../resources/sky.ppm");
    }

    return {};
}

void initialize_renderer(RendererConfig config) {
    GLOBAL_s_window_ptr = new Window(__window_render_callback, config.image_width, config.image_aspect_ratio, config.image_name);
}

void clean_up_renderer() {
    delete GLOBAL_s_window_ptr;
    GLOBAL_s_window_ptr = nullptr;
}

void load_scene() {

}
void render_scene() {
    if (GLOBAL_s_window_ptr->create_opengl_window()) {
        GLOBAL_s_window_ptr->launch_window_loop();
    } else {
        std::cerr << "Error: couldnot create an opengl window\n";
        /// TODO: print to a ppm file instead (or use stb_image_write for PNG).
    }
}

} // namespace renderer