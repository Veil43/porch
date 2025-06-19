#include "aabb.hh"

AABB::AABB() {}
AABB::AABB(const math::Interval& x, const math::Interval& y, const math::Interval& z) 
    : m_x_bounds{x}, m_y_bounds{y}, m_z_bounds{z}
{}

AABB::AABB(const point3& min, const point3& max) {
    m_x_bounds = math::Interval(std::fmin(min.x, max.x), std::fmax(min.x, max.x));
    m_y_bounds = math::Interval(std::fmin(min.y, max.y), std::fmax(min.y, max.y));
    m_z_bounds = math::Interval(std::fmin(min.z, max.z), std::fmax(min.z, max.z));
}

AABB::AABB(const AABB& box0, const AABB& box1) {
    m_x_bounds = math::Interval(box0.m_x_bounds, box1.m_x_bounds);
    m_y_bounds = math::Interval(box0.m_y_bounds, box1.m_y_bounds);
    m_z_bounds = math::Interval(box0.m_z_bounds, box1.m_z_bounds);
}

const math::Interval& AABB::axis_interval(i32 n) const {
    if (n == 1) return m_y_bounds;
    if (n == 2) return m_z_bounds;
    return m_x_bounds;
}

bool AABB::hit(const ray& r, math::Interval ray_t) const {
    const point3& ray_origin = r.origin();
    const vec3& ray_direction = r.direction();
    
    /*
        axis = 0 gives interval m_x_bounds
        axis = 1 gives interval m_y_bounds
        axis = 2 gives interval m_z_bounds
    */

    for (i32 axis = 0; axis < 3; axis++) {
        const math::Interval& axis_bounds = axis_interval(axis);
        const f64&            axis_dir_inv = 1.0/ray_direction[axis];

        f64 t0 = (axis_bounds.m_min - ray_origin[axis]) * axis_dir_inv;
        f64 t1 = (axis_bounds.m_max - ray_origin[axis]) * axis_dir_inv;

        if (t0 > t1) {
            std::swap(t0, t1);
        }

        if (t0 > ray_t.m_min) ray_t.m_min = t0;
        if (t1 < ray_t.m_max) ray_t.m_max = t1;

        if (ray_t.m_max <= ray_t.m_min) {
            return false;
        }
    }

    return true; // we overlapped
}

i32 AABB::longest_axis() const {
    if (m_x_bounds.size() > m_y_bounds.size()) {
        return m_x_bounds.size() > m_z_bounds.size() ? 0 : 2;
    } else {
        return m_y_bounds.size() > m_z_bounds.size() ? 1 : 2;
    }
}