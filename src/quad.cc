#include "quad.hh"
#include "utils.hh"

Quad::Quad(const point3& origin, 
           const vec3& u, const vec3& v, 
           std::shared_ptr<Material> material) 
    : m_origin{origin}, m_u_vector{u}, m_v_vector{v}, m_material{material}
{
    vec3 plane_normal = cross(m_u_vector, m_v_vector);
    m_normal = normalize(plane_normal);
    m_plane_d = dot(m_normal, m_origin);
    m_w_vector = plane_normal / dot(plane_normal, plane_normal);

    set_bounding_box();
}

void Quad::set_bounding_box() {
    AABB bbox_diagonal1 = AABB(m_origin, m_origin + m_u_vector + m_v_vector);
    AABB bbox_diagonal2 = AABB(m_origin + m_u_vector, m_origin + m_v_vector);
    m_bbox = AABB(bbox_diagonal1, bbox_diagonal2);
}

AABB Quad::bounding_box() const {
    return m_bbox;
}

bool Quad::hit(const ray& r, math::Interval ray_t, HitRecord& record) const {
    /// NOTE: ray-plane intersetion is given by
    // t = (D - dot(n,O))/dot(n,d)

    f64 norm_dot_dir = dot(m_normal, r.direction());
    if (std::fabs(norm_dot_dir) < 1e-8) {
        return false;
    }

    // -------------------------
    // Ray plane intersection
    // -------------------------
    f64 t = (m_plane_d - dot(m_normal, r.origin())) / norm_dot_dir;
    if (!ray_t.in_excl(t)) {
        return false;
    }

    // ---------------------------
    // Parallelogram interior test
    // ---------------------------
    point3 intersection = r.P(t);
    vec3 planar_hit_point_vector = intersection - m_origin;
    f64 a = dot(m_w_vector, cross(planar_hit_point_vector, m_v_vector));
    f64 b = dot(m_w_vector, cross(m_u_vector, planar_hit_point_vector));

    if (!is_interior(a, b, record)) {
        return false;
    }

    record.t = t;
    record.p = intersection;
    record.material = m_material;
    record.set_face_normal(r, m_normal);

    return true;
}

bool Quad::is_interior(f64 a, f64 b, HitRecord& record) const {
    math::Interval unit_interval = math::Interval(0,1);

    if (!unit_interval.in_excl(a) || !unit_interval.in_excl(b)) {
        return false;
    }

    record.u = a;
    record.v = b;

    return true;
}