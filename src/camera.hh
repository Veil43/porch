#ifndef CAMERA_HH
#define CAMERA_HH

#include "hittable.hh"
#include "color.hh"
#include "utils.hh"

class Camera {
public:
    f64 m_image_aspect_ratio    = 1.0;
    i32 m_image_width           = 256;
    i32 m_samples_per_pixel     = 10;
    i32 m_max_bounces           = 10;
    f64 m_vfov                  = 90.0;
    point3 m_position           = point3(0.0,0.0,0.0);
    point3 m_target             = point3(0.0,0.0,-1.0);
    vec3 m_world_up             = vec3(0.0,1.0,0.0);
    f64 m_defocus_angle         = 0.0;
    f64 m_focus_distance        = 10.0;
    color m_background          = color(0.0);

    utils::Image render(const Hittable& scene);

private:
    int m_image_height;
    point3 m_pixel00;
    vec3 m_delta_u;
    vec3 m_delta_v;
    vec3 uu, vv, ww;
    vec3 m_defocus_disk_u, m_defocus_disk_v;

    void initialize();
    color compute_ray_color(const ray& r, const Hittable& scene) const;
    ray generate_random_ray_for_pixel(i32 x, i32 y) const;
    /*
        returns a point in the unit square x: [-0.5,0.5], y: [-0.5,0.5]
    */
    vec3 get_random_point_in_square() const; // TODO: consider moving this to the math header
    point3 defocus_disk_sample() const;
};

#endif // CAMERA_HH