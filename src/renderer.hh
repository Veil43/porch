#ifndef RENDERER_HH
#define RENDERER_HH

#include "config.def"
#include "types.hh"

#include <string>

enum class eTargetOption : u8 {
    kScreen,
    kImageFile,
    kScreenAndFile,
};

struct RendererConfig {
    std::string image_name = DEFAULT_WINDOW_NAME;
    f32 image_width = DEFAULT_WINDOW_WIDTH;
    f32 image_aspect_ratio = DEFAULT_WINDOW_ASPECT_RATIO;
    eTargetOption target_option = eTargetOption::kScreen;
};

namespace renderer
{

void initialize_renderer(RendererConfig config = {});
void clean_up_renderer();
void load_scene();
void render_scene();

RendererConfig get_config();
void set_config(RendererConfig);
} // namespace renderer

#endif // RENDERER_HH