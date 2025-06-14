#ifndef HITTABLE_HH
#define HITTABLE_HH

#include "ray.hh"
#include "math.hh"
#include "aabb.hh"

class Material;

struct HitRecord {
    point3 p;
    vec3 normal; // Is always facing opposite the ray
    f64 t;
    std::shared_ptr<Material> material;
    f64 u;
    f64 v;
    bool front_face;
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        /// NOTE: we want the ray and normal to face opposite directions
        front_face = dot(r.direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const ray& r, math::Interval ray_t, HitRecord& record) const = 0;
    virtual AABB bounding_box() const = 0;
};

#endif // HITTABLE_HH