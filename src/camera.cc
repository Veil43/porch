#include "camera.hh"
#include "math.hh"

void Camera::initialize() {
    
}

void Camera::render(const Hittable& scene) {

}

color Camera::compute_ray_color(const ray& r, const Hittable& scene) const {
    using math::Interval;

    HitRecord record;
    // if hit color in the normal else color in the sky
    bool hit_success = scene.hit(r, Interval(0.0, math::infinity), record);
    if (hit_success) {
        color object_color = 0.5 * (record.normal + color(1.0));
        return object_color;
    }

    vec3 unit_ray_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_ray_direction.y + 1.0);
    return math::lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 0.9), t);
}