// renderer.cc
// by Reuel Nkomo 2025
#include "renderer.hh"
#include "window.hh"
#include "vec3.hh"
#include "color.hh"
#include "ray.hh"
#include "math.hh"
#include "sphere.hh"
#include "hittable.hh"
#include "hittable_list.hh"
#include "camera.hh"

#include <iostream>
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

static color compute_ray_color(const ray& r, Hittable& scene) {
    using namespace math;
    HitRecord record;
    if (scene.hit(r, Interval(0.0, infinity), record)) {
        return 0.5 * color(record.normal + color(1.0)); // bring into range 0-1
    }

    vec3 unit_dir = unit_vector(r.direction());
    f64 a = 0.5 * (unit_dir.y + 1.0);
    // a==1 blue
    // a==0 = white
    color sky_tone = math::lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 0.9), a);
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

    // ------------------------------------------
    // Sample Scene
    // ------------------------------------------
    HittableList sample_scene;

    sample_scene.add(make_shared<Sphere>(point3(0.0,0.0,-1.0), 0.5)); // p=00-1, r=.5
    sample_scene.add(make_shared<Sphere>(point3(0.0,-100.5,-1.0), 100.0));
    
    // ------------------------------------------
    // Camera Config
    // ------------------------------------------
    Camera main_camera{};
    main_camera.m_aspcet_ratio = ar;
    main_camera.m_image_width = width;
    
    // ------------------------------------------
    // Render "Loop"
    // ------------------------------------------
    main_camera.render(sample_scene, dest);
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