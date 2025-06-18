#include "camera.hh"
#include "math.hh"
#include "material.hh"

void Camera::initialize() {
    /// NOTE: here width can be rounded down or height squashed to 1
    /// Thus we have to recalculate viewport ar to match the target image
    m_image_height = i32(m_image_width/m_image_aspect_ratio);
    m_image_height = (m_image_height < 1)? 1 : m_image_height;

    f64 theta = math::radians(m_vfov);
    f64 height = std::tan(theta/2);

    f64 viewport_aspect_ratio = f64(m_image_width)/m_image_height;
    /// NOTE: 2.0 as a scaling factor is picked arbitrarily
    f64 viewport_height = 2.0 * height * m_focus_distance;
    f64 viewport_width = viewport_height * viewport_aspect_ratio;

    // Find basis vectors;
    ww = normalize(m_position-m_target); // why the 'wrong' way around? right-hand space, we want to be aligned with +z
    uu = normalize(cross(m_world_up, ww));
    vv = cross(ww, uu);

    vec3 viewport_u = viewport_width * uu;
    vec3 viewport_v = viewport_height * -vv;

    m_delta_u = viewport_u/m_image_width;
    m_delta_v = viewport_v/m_image_height;
    m_pixel00 = m_position - m_focus_distance * ww
                           - 0.5 * (viewport_u + viewport_v) 
                           + 0.5 * (m_delta_u  + m_delta_v);
    // Store lense information
    f64 defocus_disk_radius = m_focus_distance * std::tan(math::radians(m_defocus_angle/2.0));
    m_defocus_disk_u = uu * defocus_disk_radius;
    m_defocus_disk_v = vv * defocus_disk_radius;
}

utils::Image Camera::render(const Hittable& scene) {
    initialize();

    utils::Image output_image = {};
    output_image.width = m_image_width;
    output_image.height = m_image_height;
    output_image.bytes_per_channel = 1;
    output_image.channel_count = 3;
    output_image.size = output_image.bytes_per_channel * output_image.channel_count *
                        output_image.width *
                        output_image.height;
    output_image.buffer = new u8[output_image.size];
    void* buffer = (void*)output_image.buffer;

    f64 color_contribution_per_sample = 1.0 / m_samples_per_pixel;

    for (int y = 0; y < m_image_height; y++) {
        for (int x = 0; x < m_image_width; x++) {
            int y_inv = m_image_height - 1 - y;
            int index = y_inv*m_image_width + x;

            // -----------------------------------------
            // Multi-sampling
            // -----------------------------------------
            color pixel_color(0.0,0.0,0.0);
            for (i32 sample = 0; sample < m_samples_per_pixel; sample++) {
                ray r = generate_random_ray_for_pixel(x, y);
                pixel_color += compute_ray_color(r, scene);
            }

            write_color_to_buffer(buffer, index, pixel_color * color_contribution_per_sample);
        }
    }
    return output_image;
}

color Camera::compute_ray_color(const ray& r, const Hittable& scene) const {
    using math::Interval;

    color output_color = color(1.0);

    ray current_ray = r;
    for (int i = 0; i < m_max_bounces; i++) {
        HitRecord record;
        bool hit_success = scene.hit(current_ray, Interval(0.001, math::infinity), record);
        if (hit_success) {
            color attenuation;
            ray scattered_ray;
            if (record.material->scatter(current_ray, record, attenuation, scattered_ray)) {
                output_color = output_color * attenuation;
                current_ray = scattered_ray;
            } else {
                // we couldn't scatter
                break;
            }

        } else {
            vec3 unit_ray_direction = normalize(current_ray.direction());
            auto t = 0.5 * (unit_ray_direction.y + 1.0);
            color sky = math::lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 0.9), t);
            return linear_to_gamma(output_color * sky);
        }
    }
    
    return color(0.0);
}

ray Camera::generate_random_ray_for_pixel(i32 x, i32 y) const {
    vec3 offset = get_random_point_in_square();
    vec3 pixel_sample = m_pixel00 
                        + (offset.x + x) * m_delta_u
                        + (offset.y + y) * m_delta_v;
    // random origin
    vec3 ray_origin = (m_defocus_angle <= 0) ? m_position : defocus_disk_sample();
    vec3 ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

point3 Camera::defocus_disk_sample() const {
    point3 point = math::generate_random_point_in_unit_disk();
    return m_position + (point.x * m_defocus_disk_u) + (point.y * m_defocus_disk_v);
}

vec3 Camera::get_random_point_in_square() const {
    return vec3(math::randf64() - 0.5, math::randf64() - 0.5, 0.0);
}