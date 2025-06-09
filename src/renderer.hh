#ifndef RENDERER_HH
#define RENDERER_HH

#include "config.def"
#include "types.hh"
#include "window.hh"
#include "vec3.hh"

#include <string>
#include <memory>

enum class eImgDest : u8 {
    kScreen,
    kImageFile,
    kScreenAndFile,
};

struct RendererConfig {
    f32 image_width         = 400.0;
    f32 image_aspect_ratio  = 1.0;
    i32 samples_per_pixel   = 10;
    i32 max_bounces         = 10;
    f32 cam_vfov            = 90.0;
    f32 cam_defocus_angle   = 0.0;
    f32 cam_focus_distance  = 10.0;
    vec3 cam_pos            = vec3(0.0);
    vec3 cam_target         = vec3(0.0,0.0,-1.0);
    vec3 cam_world_up       = vec3(0.0,1.0,0.0);
};

class Renderer {
public:
    eImgDest m_img_dst = eImgDest::kScreen;
    f32 m_window_width = DEFAULT_WINDOW_WIDTH;
    f32 m_window_aspect_ratio = DEFAULT_WINDOW_ASPECT_RATIO;
    std::string m_image_name = DEFAULT_WINDOW_NAME;
    RendererConfig m_config = {};

    Renderer(f32 width = DEFAULT_WINDOW_WIDTH, f32 aspect_ratio = DEFAULT_WINDOW_ASPECT_RATIO, const std::string& image_name = DEFAULT_WINDOW_NAME);
    ~Renderer();

    void load_scene();
    void render_scene();
    bool create_canvas(bool resizable = false);

private:
    bool m_valid_destination = false;
    SharedData m_shared_image;
    std::unique_ptr<Window> m_main_window = nullptr;
};

#endif // RENDERER_HH