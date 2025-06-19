#ifndef RAY_HH
#define RAY_HH


#include "vec3.hh"
#include "types.hh"

class ray {
public:
    ray();
    ray(const point3& origin, const vec3& direction);

    const point3& origin() const;
    const vec3& direction() const;
    point3 P(f64 t) const;

private:
    point3 m_origin;
    vec3 m_direction;
};

#endif // RAY_HH