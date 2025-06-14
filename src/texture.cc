#include "texture.hh"


SolidColor::SolidColor(const color& albedo) 
    : m_albedo{albedo}
{}

SolidColor::SolidColor(f64 red, f64 green, f64 blue) : 
    m_albedo{red, green, blue}
{}

color SolidColor::value(f64 u, f64 v, const point3& p) const {
    return m_albedo;
}

CheckerTexture::CheckerTexture(f64 scale, std::shared_ptr<SolidColor> even, std::shared_ptr<SolidColor> odd)
    : m_inv_scale{1.0 / scale}, m_even{even}, m_odd{odd}
{}

CheckerTexture::CheckerTexture(f64 scale, const color& c1, const color& c2) 
    : m_inv_scale{1.0 / scale}, m_even{std::make_shared<SolidColor>(c1)}, m_odd{std::make_shared<SolidColor>(c2)}
{}

color CheckerTexture::value(f64 u, f64 v, const point3& p) const {
    i32 floor_x = i32(std::floor(m_inv_scale * p.x));
    i32 floor_y = i32(std::floor(m_inv_scale * p.y));
    i32 floor_z = i32(std::floor(m_inv_scale * p.z));

    bool is_even = (floor_x + floor_y + floor_z) % 2 == 0;

    return is_even ? m_even->value(u, v, p) : m_odd->value(u, v, p);
}