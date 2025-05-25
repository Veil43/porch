#include "camera.hh"
#include "math.hh"

void Camera::initialize() {
    /// NOTE: here width can be rounded down or height squashed to 1
    /// Thus we have to recalculate viewport ar to match the target image
    m_image_height = i32(m_image_width/m_aspcet_ratio);
    m_image_height = (m_image_height < 1)? 1 : m_image_height;

    /// NOTE: viewport height is picked arbitrarily
    f64 viewport_aspect_ratio = f64(m_image_width)/m_image_height;
    f64 viewport_height = 2.0;
    f64 viewport_width = viewport_height * viewport_aspect_ratio;
    f64 focal_distance = 1.0;

    vec3 viewport_u = vec3(viewport_width,0.0,0.0);
    vec3 viewport_v = vec3(0.0,-viewport_height,0.0);
    m_delta_u = viewport_u/m_image_width;
    m_delta_v = viewport_v/m_image_height;
    m_pixel00 = m_position + vec3(0.0,0.0,-focal_distance) 
                           - 0.5 * (viewport_u + viewport_v) 
                           + 0.5 * (m_delta_u  + m_delta_v);
    
}

void Camera::render(const Hittable&) {}
void Camera::render(const Hittable& scene, SharedData& dest) {
    initialize();
    void* buffer = dest.data;
    for (int y = 0; y < m_image_height; y++) {
        std::lock_guard<std::mutex> lock(dest.mtx);
        for (int x = 0; x < m_image_width; x++) {
            int y_inv = m_image_height - 1 - y;
            int index = y_inv*m_image_width + x;

            vec3 pixel_center = m_pixel00 + (x*m_delta_u) + (y*m_delta_v);
            vec3 ray_dir = pixel_center - m_position;
            ray r = ray(m_position, ray_dir);

            color pixel_color = compute_ray_color(r, scene);

            write_color_to_buffer(buffer, index, pixel_color);
        }
    }
}

color Camera::compute_ray_color(const ray& r, const Hittable& scene) const {
    using math::Interval;

    HitRecord record;
    // if hit color-in the normal else color in the sky
    bool hit_success = scene.hit(r, Interval(0.0, math::infinity), record);
    if (hit_success) {
        color object_color = 0.5 * (record.normal + color(1.0));
        return object_color;
    }

    vec3 unit_ray_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_ray_direction.y + 1.0);
    return math::lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 0.9), t);
}