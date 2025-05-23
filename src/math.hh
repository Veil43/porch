#ifndef MATH_HH
#define MATH_HH

#include "vec3.hh"
#include "types.hh"

#include <limits>



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
    bool in_incl(f64 x) const { return m_min <= x && x <= m_max; }
    bool in_excl(f64 x) const { return m_min < x && x <  m_max; }

    static const Interval empty;
    static const Interval universe;
};

inline const Interval Interval::empty = Interval(+infinity, -infinity);
inline const Interval Interval::universe = Interval(-infinity, +infinity);

} // namespace math

#endif // MATH_HH