#ifndef SPHERE_HH
#define SPHERE_HH

#include "hittable.hh"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, f64 radius, std::shared_ptr<Material> material);
    bool hit(const ray& r, math::Interval ray_t, HitRecord& record) const override;
    AABB bounding_box() const override;
private:
    point3 m_center;
    f64 m_radius;
    AABB m_bbox;
    std::shared_ptr<Material> m_material;
};

#endif // SPHERE_HH