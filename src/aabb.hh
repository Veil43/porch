#pragma once

#include "math.hh"
#include "ray.hh"

class AABB {
public:
    math::Interval m_x_bounds;
    math::Interval m_y_bounds;
    math::Interval m_z_bounds;

    AABB();
    AABB(const math::Interval& x, const math::Interval& y, const math::Interval& z);
    AABB(const point3& min, const point3& max);
    AABB(const AABB& box0, const AABB& box1);
    
    // const math::Interval& axis_interval(i32 i) const;

    bool hit(const ray& r, math::Interval ray_t) const;

};