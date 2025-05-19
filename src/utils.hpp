#ifndef UITLS_HPP
#define UITLS_HPP

#include "types.hpp"
#include <string>

#ifdef PORCH_DEBUG
#include <iostream>
#ifdef _WIN32
    #define _DEBUG_BREAK __debugbreak();
#else
    #define _DEBUG_BREAK
#endif

#define GL_QUERY_ERROR(glFunction)                                                          \
    glFunction                                                                              \
    for (unsigned int err__ = glGetError(); err__ != GL_NO_ERROR; err__ = glGetError()) {   \
        std::cerr << "ERROR::OPENGL: 0x" << std::hex << err__ << std::dec                   \
                  << " in file: " << __FILE__ << " at line: " << __LINE__;                  \
        _DEBUG_BREAK                                                                        \
    }

#define MOLLY_ASSERT(expr) \
    if (!expr) {*(int *)0 = 0;}
#else
#define GL_QUERY_ERROR(glFunction) \
    glFunction
#define MOLLY_ASSERT(expr)
#endif // PORCH_DEBUG

namespace utils 
{
struct ImageData {
    u8* data;
    i32 width;
    i32 height;
    i32 channel_count;
};

ImageData load_image_data(const std::string& path, u32 n_channels = 0, bool should_flip = true);
void free_image_data(ImageData* image);
std::string load_text_from_file(const std::string& path);

} // namespace utils

#endif // UITLS_HPP