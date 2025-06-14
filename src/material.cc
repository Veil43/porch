#include "material.hh"

Lambertian::Lambertian(const color& albedo) 
    : m_texture{std::make_shared<SolidColor>(albedo)}
{}
Lambertian::Lambertian(std::shared_ptr<Texture> texture) 
    : m_texture{texture}
{}

bool Lambertian::scatter (
    const ray& r_in,
    const HitRecord& record,
    color& attenuation,
    ray& scattered
) const {
    /// NOTE: Uniform Distribution
    // vec3 direction = math::generate_random_vector_on_hemisphere(record.normal);
    /// NOTE: Lambertian Distribution of reflected rays (proportional to cos(phi))
    // vec3 direction = record.normal + math::generate_random_unit_vector();
    
    vec3 scatter_direction = record.normal + math::generate_random_unit_vector();

    /// NOTE: when there random unit vector ~= -normal we get a direction of 0
    /// This means that our scattered ray will always be p (the point of intersection).
    /// This is bad, we instead want it to scatter along the normal;
    if (scatter_direction.near_zero()) {
        scatter_direction = record.normal;
    }

    scattered = ray(record.p, scatter_direction);
    attenuation = m_texture->value(record.u, record.v, record.p);
    return true; // why are we returning anything??
}

Metal::Metal(const color& albedo, f64 fuzz)
    : m_albedo{albedo}, m_fuzz{ (fuzz < 1.0) ? fuzz : 1.0 }
{}

bool Metal::scatter  (
    const ray& r_in,
    const HitRecord& record,
    color& attenuation,
    ray& scattered
) const {
    vec3 reflection_direction = math::reflect(r_in.direction(), record.normal);
    vec3 scattered_direction = normalize(reflection_direction) + m_fuzz * math::generate_random_unit_vector();
    scattered = ray(record.p, scattered_direction);
    attenuation = m_albedo;
    bool is_reflected = (dot(scattered_direction, record.normal) > 0.0);
    return is_reflected;
}

Dielectric::Dielectric(f64 index_of_refraction) 
    : m_index_of_refraction{index_of_refraction}
{}
bool Dielectric::scatter  (
    const ray& r_in,
    const HitRecord& record,
    color& attenuation,
    ray& scattered
) const {
    attenuation = color(1.0, 1.0, 1.0);
    f64 index_of_refraction_ratio = record.front_face ? (1.0/m_index_of_refraction) : m_index_of_refraction; // NOTE: the index differs inside vs outside
    vec3 in_direction = normalize(r_in.direction());
    
    vec3 out_direction;
    f64 cos_theta = std::fmin(dot(in_direction, -record.normal), 1.0);
    f64 sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
    bool cannot_refract = (index_of_refraction_ratio * sin_theta) > 1.0;
    /// NOTE: we omitted Schlick Approximation for reflectance
    if (cannot_refract) {
        out_direction = math::reflect(in_direction, record.normal);
    } else {
        out_direction = math::refract(in_direction, record.normal, index_of_refraction_ratio);
    }

    scattered = ray(record.p, out_direction);
    return true;
}