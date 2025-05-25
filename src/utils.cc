#include "utils.hh"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

ImageData load_image_from_file(const std::string& path, u32 n_channels, bool should_flip) {
    ImageData output_data = {};

    if (should_flip) {
        stbi_set_flip_vertically_on_load(1);
    }
    
    output_data.data = stbi_load(path.c_str(), &output_data.width, &output_data.height, &output_data.channel_count, n_channels);
    if (output_data.data == nullptr) {
        std::cerr << "Error::I/O: could not load the image with path: <" << path << ">\n";
        return {};
    }

    return std::move(output_data);
}

void free_image_data(ImageData* image) {
    stbi_image_free(image->data);
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