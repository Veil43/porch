#include "renderer.hh"

#include <iostream>

#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    #define _CRT_MAP_ALLOC
    #include <cstdlib>
    #include <crtdbg.h>
#endif

int main(int argc, char** argv) {
#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 

    Renderer porch{};
    porch.m_image_width = 800.0;
    porch.m_aspect_ratio = 16.0/9.0;
    porch.m_samples_per_pixel = 100.0;
    porch.m_max_bounces = 50.0;

    porch.m_image_name = "Porch";
    if (porch.create_canvas(true)) {
        porch.render_scene();
    } else {
        std::cerr << "Could not create a render canvas!\n";
    }

    return 0;
}