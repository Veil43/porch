#ifndef VEC3_HH
#define VEC3_HH

#include "types.hh"
#include <ostream>

class vec3 {
public:
    union {
        f64 m[3];
        struct { f64 m_x, m_y, m_z; };
        struct { f64 x, y, z; };
        struct { f64 r, g, b; };
    };

    vec3();
    vec3(f64 x, f64 y, f64 z);

    vec3 operator-() const;
    f64 operator[](i32 i) const;
    f64& operator[](i32 i);

    vec3& operator+=(const vec3& v);
    vec3& operator*=(f64 t);
    vec3& operator/=(f64 t);
    f64 length() const;
    f64 length_squared() const;
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v);
inline vec3 operator+(const vec3& u, const vec3& v);
inline vec3 operator-(const vec3& u, const vec3& v);
inline vec3 operator*(const vec3& u, const vec3& v);
inline vec3 operator*(double t, const vec3& v);
inline vec3 operator*(const vec3& v, double t);
inline vec3 operator/(const vec3& v, double t);
inline double dot(const vec3& u, const vec3& v);
inline vec3 cross(const vec3& u, const vec3& v) ;
inline vec3 unit_vector(const vec3& v);

#endif // VEC3_HH