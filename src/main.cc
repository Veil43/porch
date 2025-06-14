#include "renderer.hh"
#include "camera.hh"
#include "material.hh"
#include "sphere.hh"

#include <iostream>

#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    #define _CRT_MAP_ALLOC
    #include <cstdlib>
    #include <crtdbg.h>
#endif

Scene get_world(i32 resolution, i32 spp, i32 max_bounces) {
    auto left = point3(-1.0, 0.0, -1.0);
	auto center = point3(0.0, 0.0, -1.2);
	auto right = point3(1.0, 0.0, -1.0);

	auto default_diffuse_blue_mat = make_shared<Lambertian>(color(0.1, 0.2, 0.5));
	auto default_diffuse_yellow_mat = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
	auto metallic_shiny_white_mat = make_shared<Metal>(color(0.8, 0.8, 0.8), 0.0);
	auto metallic_fuzzy3_white_mat = make_shared<Metal>(color(0.8, 0.8, 0.8), 0.3);
	auto air_bubble_mat = make_shared<Dielectric>(1.00 / 1.50); // air bubble in water(RI: 1.33) or in glass (RI: 1.5)
	auto glass_mat = make_shared<Dielectric>(1.50);

	auto material_ground = default_diffuse_yellow_mat;
	auto material_center = default_diffuse_blue_mat;
	auto material_left = glass_mat;
	auto material_right = metallic_shiny_white_mat;
	auto material_bubble = air_bubble_mat;

	auto default_sphere_ground = make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100, material_ground);
	auto default_center_sphere = make_shared<Sphere>(center, 0.5, material_center);
	auto default_left_sphere = make_shared<Sphere>(left, 0.5, material_left);
	auto default_right_sphere = make_shared<Sphere>(right, 0.5, material_right);
	auto default_left_bubble = make_shared<Sphere>(left, 0.4, material_bubble);

	HittableList world;
	world.add(default_sphere_ground);
    world.add(default_center_sphere);
    world.add(default_left_sphere);
    world.add(default_right_sphere);
	world.add(default_left_bubble);

	RendererConfig config;

	config.image_width = resolution;
	config.image_aspect_ratio = 16.0 / 9.0;
	config.samples_per_pixel = spp;
	config.max_bounces = max_bounces;
	config.cam_vfov = 90;
	config.cam_defocus_angle = 0.0;
	config.cam_focus_distance = 3.4;
	config.cam_pos = point3(0.0);
	config.cam_target = point3(0.0,0.0,-1.0);
	config.cam_world_up = vec3(0, 1, 0);
	config.cam_background = color(0.70, 0.80, 1.00);

	return Scene{world, config};
}

Scene rtweekend1(i32 resolution = 1200, i32 spp = 500, i32 max_bounces = 50) {
    HittableList world;

    auto checker = std::make_shared<CheckerTexture>(0.32, color(0.2), color(0.9));
    auto ground_material = make_shared<Lambertian>(checker);
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = math::randf64();
            point3 center(a + 0.9*math::randf64(), 0.2, b + 0.9*math::randf64());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = math::randvec3() * math::randvec3();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = math::randvec3(0.5, 1);
                    auto fuzz = math::randf64(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    RendererConfig config;

    config.image_aspect_ratio       = 16.0 / 9.0;
    config.image_width              = resolution;
    config.samples_per_pixel        = spp;
    config.max_bounces              = max_bounces;

    config.cam_vfov                 = 20;
    config.cam_pos                  = point3(13,2,3);
    config.cam_target               = point3(0,0,0);
    config.cam_world_up             = vec3(0,1,0);

    config.cam_defocus_angle        = 0.6;
    config.cam_focus_distance       = 10.0;

    return Scene{world, config};
}

int main(int argc, char** argv) {
#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 

    Renderer porch{};

    auto scene = rtweekend1(400, 100, 50);

    porch.m_window_width = 1.5f*scene.config.image_width;
    porch.m_window_aspect_ratio = scene.config.image_aspect_ratio;
    porch.m_window_name = "Porch";
 
    if (porch.create_canvas(true)) {
        porch.render_scene(scene);
    } else {
        std::cerr << "Could not create a render canvas!\n";
    }

    return 0;
}