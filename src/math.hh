#ifndef MATH_HH
#define MATH_HH

#include "vec3.hh"
#include "types.hh"

inline vec3 lerp(const vec3& u, const vec3& v, f64 t) {
    return (1.0-t)*u + t*v;
}

#endif // MATH_HH