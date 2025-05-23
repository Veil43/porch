#ifndef SPHERE_HH
#define SPHERE_HH

#include "hittable.hh"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, f64 radius);
    bool hit(const ray& r, math::Interval ray_t, HitRecord& record) const override;
private:
    point3 m_center;
    f64 m_radius;
};

#endif // SPHERE_HH