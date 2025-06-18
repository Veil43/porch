#include "utils.hh"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils 
{
std::string load_text_from_file(const std::string& path) {
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Error::I/O: could not open file with path: <" << path << ">\n";
        return "";
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

Image load_image_from_file(const std::string& path, u32 n_channels, bool should_flip) {
    Image output_data = {};

    if (should_flip) {
        stbi_set_flip_vertically_on_load(1);
    }

    output_data.buffer = stbi_load(path.c_str(), &output_data.width, &output_data.height, &output_data.channel_count, n_channels);
    if (output_data.buffer == nullptr) {
        std::cerr << "Error::I/O: could not load the image with path: <" << path << ">\n";
        return {};
    }

    return std::move(output_data);
}


void write_ppm3(const Image& image, std::filesystem::path path) {
    
}

void write_image_to_file(const Image& image, std::filesystem::path path, eImageFormat format) {

    std::string name = path.string() + image.name;
    switch (format) {
        case eImageFormat::kPPM6: {
            stbi_write_bmp(name.c_str(), image.width, image.height, image.channel_count, image.buffer);
        } break;
        case eImageFormat::kJPG: {
            stbi_write_jpg(name.c_str(), image.width, image.height, image.channel_count, image.buffer, 100);
        } break;
        case eImageFormat::kPNG: {
            stbi_write_png(name.c_str(), image.width, image.height, image.channel_count, image.buffer, image.bytes_per_channel * image.channel_count);
        } break;
        default: {
            write_ppm3(image, path);
        } break;
    }
}

void free_image_data(Image* image) {
    stbi_image_free(image->buffer);
    *image = {};
}

std::string repeat(std::string input, int n) {
    for (int i = 0; i < n; i++) {
        input+=input;
    }
    return input;
}

std::string find_true_path(const std::string& origin) {
    std::string true_path = origin;
    const int search_depth = 10;
    for (int i = 0; i < search_depth; i++) {
        true_path = repeat("../", i) + origin;
        if (fs::exists(true_path)) {
            return true_path;
        }
    }
    return origin;
}

} // namespace utils