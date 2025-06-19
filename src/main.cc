#include <iostream>
#include <chrono>

#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    #define _CRT_MAP_ALLOC
    #include <cstdlib>
    #include <crtdbg.h>
#endif

#include "hittable_list.hh"
#include "camera.hh"
#include "material.hh"
#include "sphere.hh" 
#include "bvh.hh"

struct Scene {
    std::string name;
    HittableList hittables;
    Camera camera;
};

Scene sample_scene(i32 width, i32 spp, i32 max_bounces, const std::string& name = "3 Sphere default scene") {
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

	Camera main_camera;

	main_camera.m_image_width = width;
	main_camera.m_image_aspect_ratio = 16.0 / 9.0;
	main_camera.m_samples_per_pixel = spp;
	main_camera.m_max_bounces = max_bounces;
	main_camera.m_vfov = 90;
	main_camera.m_defocus_angle = 0.0;
	main_camera.m_focus_distance = 3.4;
	main_camera.m_position = point3(0.0);
	main_camera.m_target = point3(0.0,0.0,-1.0);
	main_camera.m_world_up = vec3(0, 1, 0);
	main_camera.m_background = color(0.70, 0.80, 1.00);

    Scene output_scene = {};
    output_scene.name = name;
    output_scene.hittables = world;
    output_scene.camera = main_camera;

	return output_scene;
}

Scene rtweekend1(i32 width, i32 spp, i32 max_bounces, const std::string& name = "rtweekend 1 final scene") {
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

    Camera main_camera;

    main_camera.m_image_aspect_ratio       = 16.0 / 9.0;
    main_camera.m_image_width              = width;
    main_camera.m_samples_per_pixel        = spp;
    main_camera.m_max_bounces              = max_bounces;

    main_camera.m_vfov                 = 20;
    main_camera.m_position                  = point3(13,2,3);
    main_camera.m_target               = point3(0,0,0);
    main_camera.m_world_up             = vec3(0,1,0);

    main_camera.m_defocus_angle        = 0.6;
    main_camera.m_focus_distance       = 10.0;

    Scene output_scene = {};
    output_scene.name = name;
    output_scene.hittables = world;
    output_scene.camera = main_camera;

	return output_scene;
}


class Renderer {
public:
    utils::Image render_image(Scene scene, const std::string& name, bool bvh = false) {
        auto start = std::chrono::high_resolution_clock::now();
        
        utils::Image output_image = {};
        if (bvh) {
            auto bvh_hittables = HittableList(make_shared<BVHNode>(scene.hittables));
            output_image = scene.camera.render(bvh_hittables);
        } else {
            output_image = scene.camera.render(scene.hittables);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Rendering took: " << elapsed.count() / 1000.0f << " seconds\n";

        output_image.name = name;
        return output_image;
    }
};

int main(int argc, char** argv) {
#if defined(PORCH_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 
    auto scene = rtweekend1(1200, 500, 50, "rtweekend1.w1200s500b50");
    
    Renderer porch = {};
    auto image = porch.render_image(scene, scene.name, true);
    utils::write_image_to_file(image, "../out/", utils::eImageFormat::kPNG);

    return 0;
}