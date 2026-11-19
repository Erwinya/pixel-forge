#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace pixelforge {

struct Image {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> rgb;  // size = width * height * 3
};

void validate_image(const Image &img);

Image load_ppm(const std::string &path);
void save_ppm(const std::string &path, const Image &img);

Image to_grayscale(const Image &img);
Image box_blur(const Image &img, int radius);
Image sobel_edges(const Image &img);
Image threshold(const Image &img, uint8_t cutoff);

}  // namespace pixelforge
