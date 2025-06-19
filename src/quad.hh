#pragma once

#include "hittable.hh"

#include <memory>

#ifdef PORCH_DEBUG
inline size_t parallel_to_plane = 0;
inline size_t too_close_to_plane = 0;
inline size_t outside_quad = 0;
#endif

class Quad : public Hittable {
public:
    Quad(const point3& origin, const vec3& u, const vec3& v, std::shared_ptr<Material> material);

    virtual void set_bounding_box();

    AABB bounding_box() const override;
    bool hit(const ray& r, math::Interval ray_t, HitRecord& rec) const override;
    bool is_interior(f64 a, f64 b, HitRecord& record) const;
private:
    AABB m_bbox;
    point3 m_origin;
    vec3 m_u_vector;
    vec3 m_v_vector;
    vec3 m_normal;
    vec3 m_w_vector;
    f64 m_plane_d;
    std::shared_ptr<Material> m_material;
};