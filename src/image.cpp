#include "image.hpp"

#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace pixelforge {
namespace {

void skip_comments(std::istream &in) {
    while (true) {
        in >> std::ws;
        if (in.peek() != '#') break;
        std::string line;
        std::getline(in, line);
    }
}

int idx(const Image &img, int x, int y, int c) {
    return (y * img.width + x) * 3 + c;
}

}  // namespace

Image load_ppm(const std::string &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot open: " + path);

    std::string magic;
    skip_comments(in);
    in >> magic;
    if (magic != "P3" && magic != "P6") {
        throw std::runtime_error("unsupported PPM magic: " + magic);
    }
    skip_comments(in);
    int w = 0, h = 0, maxv = 0;
    in >> w >> h;
    skip_comments(in);
    in >> maxv;
    if (w <= 0 || h <= 0 || maxv <= 0) throw std::runtime_error("invalid PPM header");
    if (magic == "P6") in.get();

    Image img;
    img.width = w;
    img.height = h;
    img.rgb.resize(static_cast<std::size_t>(w * h * 3));

    if (magic == "P3") {
        for (std::size_t i = 0; i < img.rgb.size(); ++i) {
            int v = 0;
            in >> v;
            if (!in) throw std::runtime_error("truncated PPM data");
            img.rgb[i] = static_cast<uint8_t>(std::clamp(v * 255 / maxv, 0, 255));
        }
    } else {
        in.read(reinterpret_cast<char *>(img.rgb.data()),
                static_cast<std::streamsize>(img.rgb.size()));
        if (!in) throw std::runtime_error("truncated PPM binary data");
        if (maxv != 255) {
            for (uint8_t &v : img.rgb) {
                v = static_cast<uint8_t>(v * 255 / maxv);
            }
        }
    }
    return img;
}

void save_ppm(const std::string &path, const Image &img) {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("cannot write: " + path);
    out << "P6\n" << img.width << ' ' << img.height << "\n255\n";
    out.write(reinterpret_cast<const char *>(img.rgb.data()),
              static_cast<std::streamsize>(img.rgb.size()));
}

Image to_grayscale(const Image &img) {
    Image out = img;
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int i = idx(img, x, y, 0);
            uint8_t r = img.rgb[i], g = img.rgb[i + 1], b = img.rgb[i + 2];
            uint8_t yv = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
            out.rgb[i] = out.rgb[i + 1] = out.rgb[i + 2] = yv;
        }
    }
    return out;
}

Image box_blur(const Image & /*img*/, int /*radius*/) {
    throw std::runtime_error("box_blur() not implemented yet");
}

Image sobel_edges(const Image & /*img*/) {
    throw std::runtime_error("sobel_edges() not implemented yet");
}

}  // namespace pixelforge
