#pragma once

#include "hittable.hh"
#include "color.hh"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const ray& r_in, 
        const HitRecord& rec, 
        color& attenuation, 
        ray& scattered
    ) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const color& albedo);
    bool scatter (
        const ray& r_in,
        const HitRecord& record,
        color& attenuation,
        ray& scattered
    ) const override;

private:
    color m_albedo;
};

class Metal : public Material {
public:
    Metal(const color& albedo);
    bool scatter (
        const ray& r_in,
        const HitRecord& record,
        color& attenuation,
        ray& scattered
    ) const override;
private:
    color m_albedo;
};