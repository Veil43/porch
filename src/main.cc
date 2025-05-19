#include "renderer.hh"

#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    #define _CRT_MAP_ALLOC
    #include <cstdlib>
    #include <crtdbg.h>
#endif

int main(int argc, char** argv) {
#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 
    // void* chunk_oh_memory = malloc(1233);

    RendererConfig config = {};
    config.image_width = 800;
    config.image_aspect_ratio = 16.0 / 9.0;
    config.image_name = "Porch Renderer";
    config.target_option = eTargetOption::kScreen;

    // Is this the best way to do this? IDK
    renderer::initialize_renderer(config);
    renderer::load_scene(); /// NOTE: no scene for now
    renderer::render_scene(); /// NOTE: just renderes a default images from file
    renderer::clean_up_renderer();

    return 0;
}