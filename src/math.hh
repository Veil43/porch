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

inline vec3 radians(f64 degrees) {
    return degrees * pi / 180.0;
}

class Interval {
public:
    f64 m_min;
    f64 m_max;

    Interval() : m_min{+infinity}, m_max{-infinity} {}
    Interval(f64 min, f64 max) : m_min{min}, m_max{max} {}

    f64 size() const { return  m_max - m_max; }
    bool in_incl(f64 value) const { return m_min <= value && value <= m_max; }
    bool in_excl(f64 value) const { return m_min < value && value <  m_max; }
    f64 clamp(f64 value) const {
        if (value < m_min) return m_min;
        if (value > m_max) return m_max;
        return value;
    }
    
    static const Interval empty;
    static const Interval universe;
};

inline const Interval Interval::empty = Interval(+infinity, -infinity);
inline const Interval Interval::universe = Interval(-infinity, +infinity);

inline f64 randf64() { return std::rand() / (RAND_MAX + 1.0); }
inline f64 randf64(f64 min, f64 max) { return min + (max-min)*randf64(); }
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
/*
    computes the relected vector assuming n is normalized
*/
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2.0*dot(v, n)*n;
}
} // namespace math

#endif // MATH_HH