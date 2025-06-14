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


bool AABB::hit(const ray& r, math::Interval ray_t) const {
    const point3& ray_origin = r.origin();
    const vec3& ray_direction = r.direction();
    f64 tmin = ray_t.m_min;
    f64 tmax = ray_t.m_max;
    
    // tn = (xn - Ox)/Dx
    /// NOTE: should add a small value eps to the numerator to avoid NaN?
    // ---------------------------------
    // x interval bounds
    // ---------------------------------
    f64 tx0 = (m_x_bounds.m_min - ray_origin.x) / ray_direction.x;
    f64 tx1 = (m_x_bounds.m_max - ray_origin.x) / ray_direction.x;
    if (tx0 > tx1) std::swap(tx0, tx1);

    tmin = std::max(tx0, tmin);
    tmax = std::min(tx1, tmax);
    if (tmax <= tmin) return false;

    // ---------------------------------
    // y interval bounds
    // ---------------------------------
    f64 ty0 = (m_y_bounds.m_min - ray_origin.y) / ray_direction.y;
    f64 ty1 = (m_y_bounds.m_max - ray_origin.y) / ray_direction.y;
    if (ty0 > ty1) std::swap(ty0, ty1);

    tmin = std::max(tx0, tmin);
    tmax = std::min(tx1, tmax);
    if (tmax <= tmin) return false;

    // ---------------------------------
    // z interval bounds
    // ---------------------------------
    f64 tz0 = (m_z_bounds.m_min - ray_origin.z) / ray_direction.z;
    f64 tz1 = (m_z_bounds.m_max - ray_origin.z) / ray_direction.z;
    if (tz0 > tz1) std::swap(tz0, tz1);
    
    tmin = std::max(tx0, tmin);
    tmax = std::min(tx1, tmax);   
    if (tmax <= tmin) return false;

    return true; // we overlapped
}

i32 AABB::longest_axis() const {
    if (m_x_bounds.size() > m_y_bounds.size()) {
        return m_x_bounds.size() > m_z_bounds.size() ? 0 : 2;
    } else {
        return m_y_bounds.size() > m_z_bounds.size() ? 1 : 2;
    }
}