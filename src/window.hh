#ifndef WINDOW_HH
#define WINDOW_HH

#include "types.hh"
#include "utils.hh"

#include <string>

struct SharedData {
    std::mutex mtx;
    u8* data;
    u32 width;
    u32 height;
    u32 channel_count;
};

class Window {
public:
    u32 m_width;
    u32 m_height;
    std::string m_name;
    bool m_resizable;

    // Window(__window_render_callback_type__* render_callback, 
    //        f32 width = kDefaultWidth, f32 aspect_ratio = kDefaultAspectRatio, 
    //        const std::string& name = kDefaultName);

    Window(SharedData& image, 
           u32 width, u32 height, 
           const std::string& name, bool resizable);

    ~Window();
    void launch_window_loop();
    bool create_opengl_window();
    
private:
    // __window_render_callback_type__* m_render_callback;
    SharedData& m_incoming_image;
    void* m_window_handle = nullptr;
    
    u32 m_vao;
    u32 m_vbo;
    u32 m_ebo;
    u32 m_shader;
    u32 m_texture_id;
};

#endif // WINDOW_HH