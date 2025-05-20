// renderer.cc
// by Reuel Nkomo 2025
#include "renderer.hh"
#include "window.hh"
#include "vec3.hh"
#include "color.hh"
#include "ray.hh"
#include "math.hh"

#include "utils.hh"

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

color ray_color(ray r) {
    vec3 unit_dir = unit_vector(r.direction());
    f64 t = 0.5 * (unit_dir.y + 1.0);
    // t==1 blue
    // t==0 = white
    color sky_tone = lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 0.9), t);
    // return color(1.0, 1.0, 1.0);
    return sky_tone;
}

static void async_render(SharedData& dest, f32 width, f32 ar) {
    if (ar <= 0.0) {
        std::cerr << "Error::Renderer: cannot render an image with aspect ratio: " << ar << "\n";
        return;
    }

    if (width <= 0.0) {
        std::cerr << "Error::Renderer: cannot render an image with width: " << width << "\n";
        return;
    }

    i32 image_width = width;
    i32 image_height = i32(image_width/ar);
    image_height = (image_height < 1)? 1 : image_height;
    
    void* buffer = nullptr;
    {
        std::lock_guard<std::mutex> lock(dest.mtx);
        size_t size = image_height * image_width * 3;
        dest.width = image_width;
        dest.height = image_height;
        dest.channel_count = 3;
        dest.data = new u8[size];
        buffer = (void*)dest.data;
    }

    /// NOTE: here width can be rounded down or height squashed to 1
    /// Thus we have to recalculate viewport ar to match the target image
    f64 viewport_aspect_ratio = f64(image_width)/image_height;
    f64 viewport_height = 2.0; /// NOTE: we explicitely set the viewport height (its arbitraty)
    f64 viewport_width = viewport_aspect_ratio * viewport_height;
    point3 camera_center = point3(0.0, 0.0, 0.0);
    f64 focal_distance = 1.0;

    vec3 viewport_u = vec3(viewport_width, 0.0, 0.0);
    vec3 viewport_v = vec3(0.0, -viewport_height, 0.0);
    vec3 u_delta = viewport_u/image_width;
    vec3 v_delta = viewport_v/image_height;
    vec3 pixel00 = camera_center + vec3(0.0, 0.0, -focal_distance) 
                   - 0.5*(viewport_u + viewport_v) 
                   + 0.5*(u_delta + v_delta);

    for (size_t y = 0; y < image_height; y++) {
        std::lock_guard<std::mutex> lock(dest.mtx);
        for (size_t x = 0; x < image_width; x++) {
            size_t y_inv = image_height -1 -y;
            size_t index = y_inv * image_width + x;

            vec3 pixel_center = pixel00 + (x*u_delta) + (y*v_delta);
            vec3 ray_dir = pixel_center - camera_center;
            ray r = ray(camera_center, ray_dir);

            color pixel_color = ray_color(r);
            // f32 r = f32(x) / (image_width - 1);
            // f32 g = 0;
            // f32 b = f32(y) / (image_height - 1);

            write_color_to_buffer((void*)buffer, index, pixel_color);
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