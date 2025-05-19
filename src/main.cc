#include "renderer.hh"

int main(int argc, char** argv) {

    RendererConfig config = {};
    config.image_width = 800;
    config.image_aspect_ratio = 16.0 / 9.0;
    config.image_name = "Porch Renderer";

    // Is this the best way to do this? IDK
    renderer::initialize_renderer(config);
    renderer::load_scene();
    renderer::render_scene();
    renderer::clean_up_renderer();

    return 0;
}