#pragma once

#include "types.hh"
#include "color.hh"

class Texture {
public:
    virtual ~Texture() = default;
    virtual color value(f64 u, f64 v, const point3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor(const color& albedo);
    SolidColor(f64 red, f64 green, f64 blue);
    color value(f64 u, f64 v, const point3& p) const override;
private:
    color m_albedo;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(f64 scale, std::shared_ptr<SolidColor> even, std::shared_ptr<SolidColor> odd);
    CheckerTexture(f64 scale, const color& c1, const color& c2);
    color value(f64 u, f64 v, const point3& p) const override;
private:
    f64 m_inv_scale;
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;
};