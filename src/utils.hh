#ifndef UITLS_HH
#define UITLS_HH

#include <string>
#include <filesystem>

#include "types.hh"

#ifdef PORCH_DEBUG
#include <iostream>
#ifdef _MSC_VER
    #define _DEBUG_BREAK __debugbreak();
#else
    #define _DEBUG_BREAK __builtin_trap();
#endif

#define GL_QUERY_ERROR(glFunction)                                                          \
    glFunction                                                                              \
    for (unsigned int err__ = glGetError(); err__ != GL_NO_ERROR; err__ = glGetError()) {   \
        std::cerr << "ERROR::OPENGL: 0x" << std::hex << err__ << std::dec                   \
                  << " in file: " << __FILE__ << " at line: " << __LINE__;                  \
        _DEBUG_BREAK                                                                        \
    }

#define ASSERT(expr) \
    if (!(expr)) {_DEBUG_BREAK}
#else
#define GL_QUERY_ERROR(glFunction) \
    glFunction
#define ASSERT(expr)
#endif // PORCH_DEBUG


namespace utils 
{

enum class eImageFormat {
    kPPM3,
    kPPM6,
    kJPG,
    kPNG
};

struct Image {
    std::string name;
    u8* buffer;
    size_t size;
    i32 width;
    i32 height;
    i32 bytes_per_channel = 1;
    i32 channel_count;
};

Image load_image_from_file(const std::string& path, u32 n_channels = 0, bool should_flip = true);
void free_image_data(Image* image);
std::string load_text_from_file(const std::string& path);
std::string find_true_path(const std::string& origin);
std::string repeat(std::string input, int n);
void write_image_to_file(const Image& image, std::filesystem::path path = "./", eImageFormat format = eImageFormat::kPNG);
void write_ppm3(const Image& image, std::filesystem::path path);

} // namespace utils

#endif // UITLS_HH