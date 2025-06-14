#pragma once

#include "hittable.hh"
#include "color.hh"
#include "texture.hh"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const ray& r_in, 
        const HitRecord& record, 
        color& attenuation, 
        ray& scattered
    ) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const color& albedo);
    Lambertian(std::shared_ptr<Texture> texture);
    bool scatter (
        const ray& r_in,
        const HitRecord& record,
        color& attenuation,
        ray& scattered
    ) const override;

private:
    std::shared_ptr<Texture> m_texture;
};

class Metal : public Material {
public:
    Metal(const color& albedo, f64 fuzz = 0.0);
    bool scatter (
        const ray& r_in,
        const HitRecord& record,
        color& attenuation,
        ray& scattered
    ) const override;
private:
    color m_albedo;
    f64 m_fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(f64 index_of_refaction);
    bool scatter (
        const ray& r_in,
        const HitRecord& record,
        color& attenuation,
        ray& scattered
    ) const override;
private:
    /// NOTE: this is the refractive index in the air OR the ratio relative to the
    /// enclosing media. It is stored as a ratio an assume IR/1.0 (for air)
    f64 m_index_of_refraction;
};