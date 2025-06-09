#ifndef COLOR_HH
#define COLOR_HH

#include "vec3.hh"

using color = vec3;
struct rgb8 { u8 r, g, b; };

void write_color_to_out(std::ostream& out, const color& pixel_color);
void write_color_to_buffer(void* buffer, size_t index, const color& pixel_color);
void write_color_to_buffer(void* buffer, int x, int y, const color& pixel_color);
color linear_to_gamma(color linear_color);

#endif // COLOR_HH