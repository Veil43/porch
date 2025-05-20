#ifndef RENDERER_HH
#define RENDERER_HH

#include "config.def"
#include "types.hh"
#include "window.hh"

#include <string>
#include <memory>

enum class eImgDest : u8 {
    kScreen,
    kImageFile,
    kScreenAndFile,
};

class Renderer {
public:
    eImgDest m_img_dst = eImgDest::kScreen;
    f32 m_image_width = DEFAULT_WINDOW_WIDTH;
    f32 m_aspect_ratio = DEFAULT_WINDOW_ASPECT_RATIO;
    std::string m_image_name = DEFAULT_WINDOW_NAME;

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