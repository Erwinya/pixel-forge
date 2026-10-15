#include "image.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
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
    if (magic == "P6") in.get();  // consume single whitespace

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

Image box_blur(const Image &img, int radius) {
    if (radius < 1) return img;
    Image out = img;
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int sum[3] = {0, 0, 0};
            int count = 0;
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    int xx = x + dx;
                    int yy = y + dy;
                    if (xx < 0 || yy < 0 || xx >= img.width || yy >= img.height) continue;
                    int i = idx(img, xx, yy, 0);
                    sum[0] += img.rgb[i];
                    sum[1] += img.rgb[i + 1];
                    sum[2] += img.rgb[i + 2];
                    ++count;
                }
            }
            int o = idx(out, x, y, 0);
            out.rgb[o] = static_cast<uint8_t>(sum[0] / count);
            out.rgb[o + 1] = static_cast<uint8_t>(sum[1] / count);
            out.rgb[o + 2] = static_cast<uint8_t>(sum[2] / count);
        }
    }
    return out;
}

Image sobel_edges(const Image &img) {
    Image gray = to_grayscale(img);
    Image out = gray;
    const int kx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int ky[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int y = 1; y < img.height - 1; ++y) {
        for (int x = 1; x < img.width - 1; ++x) {
            int gx = 0, gy = 0;
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int v = gray.rgb[idx(gray, x + i, y + j, 0)];
                    gx += kx[j + 1][i + 1] * v;
                    gy += ky[j + 1][i + 1] * v;
                }
            }
            int mag = static_cast<int>(std::min(255.0, std::sqrt(gx * gx + gy * gy)));
            int o = idx(out, x, y, 0);
            out.rgb[o] = out.rgb[o + 1] = out.rgb[o + 2] = static_cast<uint8_t>(mag);
        }
    }
    return out;
}

}  // namespace pixelforge
