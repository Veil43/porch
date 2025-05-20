#include "vec3.hh"

// -----------------------------------------------------------------
// class vec3 Members
// -----------------------------------------------------------------
vec3::vec3() 
    : m_x{0}, m_y{0}, m_z{0} 
{}
vec3::vec3(f64 x, f64 y, f64 z) 
    : m_x{x}, m_y{y}, m_z{z} 
{};

vec3 vec3::operator-() const { return vec3(-x, -y, -z); }
f64 vec3::operator[](i32 i) const { return m[i]; }
f64& vec3::operator[](i32 i) { return m[i]; }

vec3& vec3::operator+=(const vec3& v) {
    x+=v.x;
    y+=v.y;
    z+=v.z;
    return *this;
}

vec3& vec3::operator*=(f64 t) {
    x*=t;
    y*=t;
    z*=t;
    return *this;
}

vec3& vec3::operator/=(f64 t) {
    x/=t;
    y/=t;
    z/=t;
    return *this;
}

f64 vec3::length() const {
    return std::sqrt( x*x + y*y + z*z);
}

f64 vec3::length_squared() const {
    return x*x + y*y + z*z;
}

// -----------------------------------------------------------------
// class vec3 Non-Members
// -----------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(f64 t, const vec3& v) {
    return vec3(v.x*t, v.y*t, v.z*t);
}

inline vec3 operator*(const vec3& v, double t) {
    return vec3(v.x*t, v.y*t, v.z*t);
}

inline vec3 operator/(const vec3& v, double t) {
    return vec3(v.x/t, v.y/t, v.z/t);
}

inline f64 dot(const vec3& u, const vec3& v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z); 
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(
        u.y*v.z - u.z*v.y,
        v.x*u.z - u.x*v.z,
        u.x*v.y - v.x*u.y
    );
}

inline vec3 unit_vector(const vec3& v) {
    return v/v.length();
}
