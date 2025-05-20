#include "color.hh"

struct rgb8 { u8 r, g, b; };

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

void write_color_to_buffer(void* buffer, size_t index, const color& pixel_color) {
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;
    
    /// NOTE: why do we use 255.999 instead of 255?
    /// It's to compasate for tiny errors so that r = 1 - eps give 255
    u8 rbyte = u8(255.999 * r);
    u8 gbyte = u8(255.999 * g);
    u8 bbyte = u8(255.999 * b);

    rgb8* dest = (rgb8*) buffer;
    dest[index] = rgb8{rbyte, gbyte, bbyte};
}