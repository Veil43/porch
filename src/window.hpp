#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "types.hpp"
#include <string>

class Window {
public:
    float m_width = 400.0;
    float m_aspect_ratio = 1.0;
    std::string m_name = "Default Window Name";

    Window(f32 width, f32 aspect_ratio, const std::string& name);
    ~Window();
    void draw_current_buffer();
    void update_buffer(u8* buffer, size_t byte_count);
    void launch_window_loop();

private:
    void* m_window_handle = nullptr;
    u32 m_vao;
    u32 m_vbo;
    u32 m_ebo;
    u32 m_shader;
    u32 m_texture_id;
    bool create_opengl_window();
};

#endif // WINDOW_HPP