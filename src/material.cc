#include "material.hh"

Lambertian::Lambertian(const color& albedo) 
    : m_albedo{albedo}
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
    attenuation = m_albedo;
    return true; // why are we returning anything??
}

Metal::Metal(const color& albedo)
    : m_albedo{albedo}
{}

bool Metal::scatter  (
    const ray& r_in,
    const HitRecord& record,
    color& attenuation,
    ray& scattered
) const {
    vec3 scatter_direction = math::reflect(r_in.direction(), record.normal);
    scattered = ray(record.p, scatter_direction);
    attenuation = m_albedo;
    return true;
}