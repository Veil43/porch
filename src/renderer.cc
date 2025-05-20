// renderer.cc
// by Reuel Nkomo 2025
#include "renderer.hh"
#include "window.hh"
#include "utils.hh"

#include <thread>

Renderer::Renderer(f32 width, f32 aspect_ratio, const std::string& name)
    : m_shared_image{ std::mutex()}
{}

Renderer::~Renderer() {
    std::lock_guard<std::mutex> lock(m_shared_image.mtx);
}

bool Renderer::create_canvas(bool resizable) {
    switch (m_img_dst) {
        case eImgDest::kScreen: {
            u32 height = m_image_width / m_aspect_ratio;
            m_main_window = std::make_unique<Window>(m_shared_image, static_cast<u32>(m_image_width), height, m_image_name, resizable);
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

void Renderer::render_scene() {
    if (!m_valid_destination) {
        std::cerr << "Error: cannot render without a valid destination!\n";
        return;
    }

    // std::lock_guard<std::mutex> lock(m_shared_image.mtx);
    // std::thread t_render(render);
    // t_render.detach();

    auto raw = utils::load_image_from_file("../resources/sky.ppm");
    size_t size = raw.width * raw.height * raw.channel_count;
    m_shared_image.width = raw.width;
    m_shared_image.height = raw.height;
    m_shared_image.channel_count = raw.channel_count;
    m_shared_image.data = new u8[size];
    std::memcpy(m_shared_image.data, raw.data, size);
    utils::free_image_data(&raw);

    m_main_window->launch_window_loop();

    if (m_shared_image.data != nullptr) {
        delete[] m_shared_image.data;
    }
}

void Renderer::load_scene() {

}