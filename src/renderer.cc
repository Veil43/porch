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
#include "material.hh"

#include <iostream>
#include <thread>
#include <atomic>
#include <memory>

Renderer::Renderer(f32 width, f32 aspect_ratio, const std::string& name)
    : m_shared_image{std::atomic<bool>(true)}
{}

Renderer::~Renderer() {
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

struct RendererConfig {
    f32 width;
    f32 aspect_ratio;
    i32 samples_per_pixel;
    i32 max_bounces;
};

static void async_render(SharedData& dest, RendererConfig config, std::shared_ptr<std::atomic<bool>> running) {
    if (config.aspect_ratio <= 0.0) {
        std::cerr << "Error::Renderer: cannot render an image with aspect ratio: " << config.aspect_ratio << "\n";
        return;
    }

    if (config.width <= 0.0) {
        std::cerr << "Error::Renderer: cannot render an image with width: " << config.width << "\n";
        return;
    }

    i32 image_width = config.width;
    i32 image_height = i32(image_width/config.aspect_ratio);
    image_height = (image_height < 1)? 1 : image_height;
    
    dest.is_writing.store(true);
    size_t size = image_height * image_width * 3;
    dest.width = image_width;
    dest.height = image_height;
    dest.channel_count = 3;
    dest.data = new u8[size];
    dest.is_writing.store(false);

    // ------------------------------------------
    // Sample Scene
    // ------------------------------------------
    HittableList sample_scene;

    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Metal>(color(0.8, 0.8, 0.8));
    auto material_right  = make_shared<Metal>(color(0.8, 0.6, 0.2));

    sample_scene.add(make_shared<Sphere>(point3(0.0,-100.5,-1.0), 100.0, material_ground));
    sample_scene.add(make_shared<Sphere>(point3(0.0,0.0,-1.2), 0.5, material_center));
    sample_scene.add(make_shared<Sphere>(point3(-1.0,0.0,-1.0), 0.5, material_left));
    sample_scene.add(make_shared<Sphere>(point3( 1.0,0.0,-1.0), 0.5, material_right));
    
    // ------------------------------------------
    // Camera Config
    // ------------------------------------------
    Camera main_camera{};
    main_camera.m_aspcet_ratio = config.aspect_ratio;
    main_camera.m_image_width = config.width;
    main_camera.m_samples_per_pixel = config.samples_per_pixel;
    main_camera.m_max_bounces = config.max_bounces;

    // ------------------------------------------
    // Render "Loop"
    // ------------------------------------------
    main_camera.render(sample_scene, dest, running);
    dest.is_writing.store(true);
}

void Renderer::render_scene() {
    if (!m_valid_destination) {
        std::cerr << "Error: cannot render without a valid destination!\n";
        return;
    }

    auto window_running = std::make_shared<std::atomic<bool>>(true);
    RendererConfig config {
        m_image_width,
        m_aspect_ratio,
        m_samples_per_pixel,
        m_max_bounces,
    };

    std::thread t_render(async_render, std::ref(m_shared_image), config, window_running);
    m_main_window->launch_window_loop(m_shared_image);
    window_running->store(false); 

    t_render.join();

    if (m_shared_image.data != nullptr) {
        delete[] m_shared_image.data;
    }
}

void Renderer::load_scene() {

}