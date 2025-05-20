// renderer.cc
// by Reuel Nkomo 2025
#include "renderer.hh"
#include "window.hh"
#include "utils.hh"
#include "vec3.hh"
#include "color.hh"

#include <thread>

Renderer::Renderer(f32 width, f32 aspect_ratio, const std::string& name)
    : m_shared_image{ std::mutex()}
{}

Renderer::~Renderer() {
    std::lock_guard<std::mutex> lock(m_shared_image.mtx);
}

bool Renderer::create_canvas(bool resizable) {

    u32 height = m_image_width / m_aspect_ratio;
    m_shared_image.width = m_image_width;
    m_shared_image.height = height;
    m_shared_image.channel_count = 3;
    m_shared_image.data = nullptr;

    switch (m_img_dst) {
        case eImgDest::kScreen: {
            m_main_window = std::make_unique<Window>(static_cast<u32>(m_image_width), height, m_image_name, resizable);
            if (m_main_window) {
                m_valid_destination = m_main_window->create_opengl_window();
                return m_valid_destination;
            }
        } break;

        case eImgDest::kImageFile: {
            /// TODO: Add image writing
            std::cerr << "Error: image writing not supported yet\n";
            return false;
        } break;

        default: {
            /// TODO: Add image writing
            std::cerr << "Error: image writing not supported yet!\n";
            return false;
        }
    }

    return false;
}

static void async_render(SharedData& dest, f32 width, f32 ar) {
#ifdef RENDER_PLAIN
    auto raw = utils::load_image_from_file("../resources/sky.ppm");
    size_t size = raw.width * raw.height * raw.channel_count;
    {
        std::lock_guard<std::mutex> lock(dest.mtx);
        
        dest.width = raw.width;
        dest.height = raw.height;
        dest.channel_count = raw.channel_count;
        dest.data = new u8[size];
    }
    std::memcpy(dest.data, raw.data, size);
    utils::free_image_data(&raw);
#endif
    u32 h = width / ar;
    u32 w = h * ar;

    void* buffer = nullptr;
    {
        std::lock_guard<std::mutex> lock(dest.mtx);
        size_t size = h * w * 3;
        dest.width = w;
        dest.height = h;
        dest.channel_count = 3;
        dest.data = new u8[size];
        buffer = (void*)dest.data;
    }

    u32 image_width = w;
    u32 image_height = h;

    for (size_t y = 0; y < image_height; y++) {
        std::lock_guard<std::mutex> lock(dest.mtx);
        for (size_t x = 0; x < image_width; x++) {
            size_t y_inv = h -1 - y;
            size_t index = y_inv * image_width + x;

            f32 r = f32(x) / (image_width - 1);
            f32 g = f32(y) / (image_height - 1);
            f32 b = 0;

            write_color_to_buffer((void*)buffer, index, color{r, g, b});
        }
    }
}

void Renderer::render_scene() {
    if (!m_valid_destination) {
        std::cerr << "Error: cannot render without a valid destination!\n";
        return;
    }

    std::thread t_render(async_render, std::ref(m_shared_image), m_image_width, m_aspect_ratio);
    m_main_window->launch_window_loop(m_shared_image);

    t_render.join();

    if (m_shared_image.data != nullptr) {
        delete[] m_shared_image.data;
    }
}

void Renderer::load_scene() {

}