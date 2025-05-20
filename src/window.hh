#ifndef WINDOW_HH
#define WINDOW_HH

#include "types.hh"
#include <string>
#include <mutex>

struct SharedData {
    std::mutex mtx;
    u8* data = nullptr;
    u32 width = 0;
    u32 height = 0;
    u32 channel_count = 0;
};

class Window {
public:
    u32 m_width;
    u32 m_height;
    std::string m_name;
    bool m_resizable;

    Window(u32 width, u32 height, 
           const std::string& name, bool resizable);

    ~Window();
    void launch_window_loop(SharedData& source);
    bool create_opengl_window();
    
private:
    void* m_window_handle = nullptr;
    
    u32 m_vao;
    u32 m_vbo;
    u32 m_ebo;
    u32 m_shader;
    u32 m_texture_id;
};

#endif // WINDOW_HH