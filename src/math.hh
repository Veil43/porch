#ifndef MATH_HH
#define MATH_HH

#include "vec3.hh"
#include "types.hh"

#include <limits>
#include <cstdlib>
#include <random>

namespace math 
{

// -----------------------------------------
// Constants
// -----------------------------------------
inline const f64 infinity = std::numeric_limits<f64>::infinity();
inline const f64 pi =  3.1415926535897932385;

inline vec3 lerp(const vec3& u, const vec3& v, f64 t) {
    return (1.0-t)*u + t*v;
}

inline f64 radians(f64 degrees) {
    return degrees * pi / 180.0;
}

class Interval {
public:
    f64 m_min;
    f64 m_max;

    Interval() : m_min{+infinity}, m_max{-infinity} {}
    Interval(f64 min, f64 max) : m_min{min}, m_max{max} {}
    Interval(const Interval& a, const Interval& b) {
        m_min = std::min(a.m_min, b.m_min);
        m_max = std::max(a.m_max, b.m_max);
    }

    f64 size() const { return  m_max - m_max; }
    bool in_incl(f64 value) const { return m_min <= value && value <= m_max; }
    bool in_excl(f64 value) const { return m_min < value && value <  m_max; }
    f64 clamp(f64 value) const {
        if (value < m_min) return m_min;
        if (value > m_max) return m_max;
        return value;
    }
    Interval expand(f64 delta) const {
        f64 padding = delta/2.0;
        return Interval(m_min - padding, m_max + padding);
    }
    
    static const Interval empty;
    static const Interval universe;
};

inline const Interval Interval::empty = Interval(+infinity, -infinity);
inline const Interval Interval::universe = Interval(-infinity, +infinity);

inline f64 randf64() { return std::rand() / (RAND_MAX + 1.0); }
inline f64 randf64(f64 min, f64 max) { return min + (max-min)*randf64(); }
inline i32 randi32(i32 min, i32 max) { return i32(randf64(min, max+1)); }
inline vec3 randvec3() { return vec3(randf64(), randf64(), randf64()); }
inline vec3 randvec3(f64 min, f64 max) { return vec3(randf64(min, max), randf64(min, max), randf64(min, max)); }
inline vec3 generate_random_unit_vector() {
    /// NOTE: I believe this is a Las Vegas algorithm
    /// NOTE: We are interested in a normal vector, so why bother with 
    /// rejecting vectors that are "too long" or really, outside the unit sphere?
    /// This has to do with the fact that we want a uniform distribution of vectors,
    /// random vectors in the unit cube [-1,1] in R3 tend to be skewed towards the diagonals
    /// so we need to restrict our space to only the unit spehre.
    /// This is: << Rejection Sampling >>
    while (true) {
        vec3 vec = randvec3(-1.0, 1.0);
        f64 lensq = vec.length_squared();
        f64 eps = 1e-160;
        if (eps < lensq && lensq <= 1.0) {
            return vec / std::sqrt(lensq);
        }
    }
}
inline vec3 generate_random_vector_on_hemisphere(const vec3& normal) {
    vec3 vector_on_unit_sphere = generate_random_unit_vector();
    if (dot(vector_on_unit_sphere, normal) > 0.0) {
        return vector_on_unit_sphere;
    } else {
        return -vector_on_unit_sphere;
    }
}
inline point3 generate_random_point_in_unit_disk() {
    while (true) {
        point3 point = point3(randf64(-1,1), randf64(-1,1), 0.0);
        if (point.length_squared() < 1.0) {
            return point;
        }
    }
}

/*
    computes the reflected vector assuming n is normalized
*/
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2.0*dot(v, n)*n;
}
/*
    computes the refracted vector assuming n is normalized
    and ray_in is normalized
*/
inline vec3 refract(const vec3& ray_in, const vec3& n, f64 index_of_refraction_ratio) {
    f64 cos_theta = std::fmin(dot(ray_in, -n), 1.0); // why min?
    vec3 ray_out_perp = index_of_refraction_ratio * (ray_in + cos_theta * n);
    vec3 ray_out_par  = -std::sqrt(std::fabs(1.0 - ray_out_perp.length_squared())) * n;
    return ray_out_perp + ray_out_par;
}
inline f64 clamp(f64 value) {
    if (value < 0.0) return 0.0;
    if (value > 1.0) return 1.0;
    return value;
}

} // namespace math

#endif // MATH_HH