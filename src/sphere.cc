#include "sphere.hh"

Sphere::Sphere(const point3& center, f64 radius) 
    : m_center{center}, m_radius{std::fmax(0.0,radius)}
{}

bool Sphere::hit(const ray& r, math::Interval ray_t, HitRecord& record) const {
    

    vec3 oc = m_center - r.origin();
    f64 a = r.direction().length_squared();
    f64 h = dot(r.direction(), oc);
    f64 c = oc.length_squared() - m_radius*m_radius;
    f64 delta = h*h - a*c;

    if (delta < 0.0) {
        return false;
    } 

    f64 sqrt_delta = std::sqrt(delta);
    f64 t = (h - sqrt_delta) / a;

    if (!ray_t.in_excl(t)) {
        t = (h + sqrt_delta) / a;
        if (!ray_t.in_excl(t)) {
            return false;
        }
    }

    record.t = t;
    record.p = r.P(t);
    // Rember for a sphere unit Normal N at point P is N = (P-C)/r (draw to understand)
    vec3 normal = (record.p - m_center) / m_radius;
    record.set_face_normal(r, normal);
    return true;
}