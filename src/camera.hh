#ifndef CAMERA_HH
#define CAMERA_HH

#include "hittable.hh"
#include "color.hh"
#include "window.hh"

class Camera {
public:
    f64 m_aspcet_ratio = 1.0;
    i32 m_image_width = 256;
    void render(const Hittable& scene);
    void render(const Hittable& scene, SharedData& buffer);

private:
    int m_image_height;
    point3 m_position;
    point3 m_pixel00;
    vec3 m_delta_u;
    vec3 m_delta_v;

    void initialize();
    color compute_ray_color(const ray& r, const Hittable& scene) const;
};

#endif // CAMERA_HH