#include "ray.hh"

ray::ray() {};
ray::ray(const point3& origin, const  vec3& direction)
    : m_origin{origin}, m_direction{direction}
{}
const point3& ray::origin() const { return m_origin; }
const vec3& ray::direction() const { return m_direction; }
point3 ray::P(f64 t) const { return m_origin + t*m_direction; }