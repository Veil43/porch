// renderer.cc
// by Reuel Nkomo 2025
#include "renderer.hh"
#include "window.hh"

namespace renderer
{

// --------------------------------------------------------
// Helper
// --------------------------------------------------------
static DECL_WINDOW_RENDER_CALLBACK(__window_render_callback) {
    static constexpr bool tick = true;
    if (tick) {
        return utils::load_image_from_file("../resources/sky.ppm");
    }
    return {};
}

// --------------------------------------------------------
// Public API
// --------------------------------------------------------
/// NOTE: Find out when thi is initialized
static Window* g_s_window_ptr;
static bool s_g_print_to_file = false;
void initialize_renderer(RendererConfig config) {
    auto o = config.target_option;
    if (o == eTargetOption::kScreenAndFile) {
        g_s_window_ptr = new Window(__window_render_callback, config.image_width, config.image_aspect_ratio, config.image_name);
        s_g_print_to_file = true;
        return;
    }

    if (o == eTargetOption::kScreen) {
        g_s_window_ptr = new Window(__window_render_callback, config.image_width, config.image_aspect_ratio, config.image_name);
        return;
    }

    if (o == eTargetOption::kImageFile) {
        s_g_print_to_file = true;
        return;
    }
}

void clean_up_renderer() {
    delete g_s_window_ptr;
    g_s_window_ptr = nullptr;
}

void load_scene() {
    /// TODO: What do you even mean by "scene"?
}

void render_scene() {
    /// TODO: were we given a scene??
    if (g_s_window_ptr != nullptr && g_s_window_ptr->create_opengl_window()) {
        g_s_window_ptr->launch_window_loop();
    } else {
        s_g_print_to_file = true;
        std::cerr << "Warn: cant find a screen, defaulting to image!\n";
    }

    if (s_g_print_to_file) {
        /// TODO: print to image
        std::cerr << "Error: cant print to file just yet!\n";
        return;
    }
}

// --------------------------------------------------------
// Renderer
// --------------------------------------------------------
static void render(/*Scene*/);

} // namespace renderer