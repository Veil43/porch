#include "color.hh"
#include "math.hh"

color linear_to_gamma(color linear_color) {
    static const math::Interval clamp01(0.0, 1.0);
    color output_color = color(0.0);
    output_color.x = std::sqrt(clamp01.clamp(linear_color.x));
    output_color.y = std::sqrt(clamp01.clamp(linear_color.y));
    output_color.z = std::sqrt(clamp01.clamp(linear_color.z));

    return output_color;
}

void write_color_to_out(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;
    
    /// NOTE: why do we use 255.999 instead of 255?
    /// It's to compasate for tiny errors so that r = 1 - eps give 255
    u8 rbyte = u8(255.999 * r);
    u8 gbyte = u8(255.999 * g);
    u8 bbyte = u8(255.999 * b);
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

void write_color_to_buffer(void* buffer, int x, int y, const color& pixel_color) {
}

void write_color_to_buffer(void* buffer, size_t index, const color& pixel_color) {
    using namespace math;

    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;
    
    static const Interval intensity(0.0,0.999);
    /// NOTE: why do we use 255.999 instead of 255?
    /// It's to compensate for tiny errors so that r = 1 - eps give 255
    u8 rbyte = u8(255.999 * intensity.clamp(r));
    u8 gbyte = u8(255.999 * intensity.clamp(g));
    u8 bbyte = u8(255.999 * intensity.clamp(b));

    rgb8* dest = (rgb8*) buffer;
    dest[index] = rgb8{rbyte, gbyte, bbyte};
}