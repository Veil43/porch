#ifndef CAMERA_HH
#define CAMERA_HH

#include "hittable.hh"
#include "color.hh"

class Camera {
public:
    void render(const Hittable& world);

private:
    int m_image_height;
    point3 m_center;
    point3 pixel00;
    vec3 delta_u;
    vec3 delta_v;

    void initialize();
    color compute_ray_color(const ray& r, const Hittable& scene) const;
};

#endif // CAMERA_HH