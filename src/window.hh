#ifndef WINDOW_HH
#define WINDOW_HH

#include "types.hh"
#include "utils.hh"
#include "config.def"

#include <string>

#define DECL_WINDOW_RENDER_CALLBACK(name) utils::ImageData name(void)
typedef DECL_WINDOW_RENDER_CALLBACK(__window_render_callback_type__);

class Window {
static constexpr f32 kDefaultWidth = DEFAULT_WINDOW_WIDTH;
static constexpr f32 kDefaultAspectRatio = DEFAULT_WINDOW_ASPECT_RATIO;
static constexpr const char* kDefaultName = DEFAULT_WINDOW_NAME;
public:
    float m_width;
    float m_aspect_ratio;
    std::string m_name;

    Window(__window_render_callback_type__* render_callback, 
           f32 width = kDefaultWidth, f32 aspect_ratio = kDefaultAspectRatio, 
           const std::string& name = kDefaultName);

    ~Window();
    void launch_window_loop();
    bool create_opengl_window();
    
private:
    utils::ImageData m_surface = {};
    __window_render_callback_type__* m_render_callback;
    void* m_window_handle = nullptr;
    
    u32 m_vao;
    u32 m_vbo;
    u32 m_ebo;
    u32 m_shader;
    u32 m_texture_id;
};

#endif // WINDOW_HH