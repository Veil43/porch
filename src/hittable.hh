#ifndef HITTABLE_HH
#define HITTABLE_HH

#include "ray.hh"
#include "math.hh"

struct HitRecord {
    point3 p;
    vec3 normal; // Is always facing opposite the ray
    f64 t;
    bool front_face;
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        /// NOTE: we want the ray and normal to face opposite directions
        front_face = dot(r.direction(), outward_normal);
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const ray& r, math::Interval ray_t, HitRecord& record) const = 0;
};

#endif // HITTABLE_HH