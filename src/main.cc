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
    porch.m_window_width = 800.0;
    porch.m_window_aspect_ratio = 16.0/9.0;
    porch.m_image_name = "Porch";
    porch.m_config.image_aspect_ratio = 16.0/9.0;
    porch.m_config.image_width = 600.0;
    porch.m_config.samples_per_pixel = 50.0;
    porch.m_config.max_bounces = 25.0;
    porch.m_config.cam_pos = vec3(-1.0,4.0,1.0);
    porch.m_config.cam_target = vec3(0.0,0.0,-1.0);
    porch.m_config.cam_vfov = 20.0;
    porch.m_config.cam_defocus_angle = 10.0;
    porch.m_config.cam_focus_distance = 5.4;

    if (porch.create_canvas(true)) {
        porch.render_scene();
    } else {
        std::cerr << "Could not create a render canvas!\n";
    }

    return 0;
}