#include "vec3.hh"

// -----------------------------------------------------------------
// class vec3 Members
// -----------------------------------------------------------------
vec3::vec3() 
    : m_x{0}, m_y{0}, m_z{0} 
{}
vec3::vec3(f64 x, f64 y, f64 z) 
    : m_x{x}, m_y{y}, m_z{z} 
{}
vec3::vec3(f64 val) 
    : m_x{val}, m_y{val}, m_z{val}
{}
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