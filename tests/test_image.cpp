#include "image.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

int fails = 0;

void expect(bool cond, const std::string &msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << '\n';
        ++fails;
    }
}

std::string temp_path(const std::string &name) {
    return "build/" + name;
}

void write_text(const std::string &path, const std::string &body) {
    std::ofstream out(path, std::ios::binary);
    out << body;
}

}  // namespace

int main() {
    std::filesystem::create_directories("build");

    {
        pixelforge::Image img;
        img.width = 2;
        img.height = 1;
        img.rgb = {255, 0, 0, 0, 255, 0};
        auto gray = pixelforge::to_grayscale(img);
        expect(gray.rgb[0] == gray.rgb[1] && gray.rgb[1] == gray.rgb[2], "gray channels equal");
        expect(gray.rgb[0] > 50, "red luminance nonzero");
    }

    {
        pixelforge::Image img;
        img.width = 2;
        img.height = 2;
        img.rgb = {
            0, 0, 0, 255, 255, 255,
            255, 255, 255, 0, 0, 0,
        };
        auto blurred = pixelforge::box_blur(img, 0);
        expect(blurred.rgb == img.rgb, "radius 0 is identity");
        auto soft = pixelforge::box_blur(img, 1);
        expect(soft.rgb[0] > 0 && soft.rgb[0] < 255, "blur averages neighbors");
    }

    {
        pixelforge::Image img;
        img.width = 3;
        img.height = 3;
        img.rgb.assign(27, 0);
        // Right column white → vertical edge through the interior.
        for (int y = 0; y < 3; ++y) {
            const int i = (y * 3 + 2) * 3;
            img.rgb[i] = img.rgb[i + 1] = img.rgb[i + 2] = 255;
        }
        auto edges = pixelforge::sobel_edges(img);
        expect(edges.rgb[12] > 0, "sobel reacts to vertical edge");
    }

    {
        pixelforge::Image img;
        img.width = 2;
        img.height = 1;
        img.rgb = {10, 10, 10, 200, 200, 200};
        auto low = pixelforge::threshold(img, 128);
        expect(low.rgb[0] == 0 && low.rgb[3] == 255, "threshold splits by cutoff");
        auto high = pixelforge::threshold(img, 250);
        expect(high.rgb[0] == 0 && high.rgb[3] == 0, "high cutoff blacks both");
    }

    {
        const std::string path = temp_path("roundtrip.ppm");
        pixelforge::Image img;
        img.width = 2;
        img.height = 1;
        img.rgb = {1, 2, 3, 4, 5, 6};
        pixelforge::save_ppm(path, img);
        auto loaded = pixelforge::load_ppm(path);
        expect(loaded.width == 2 && loaded.height == 1, "roundtrip size");
        expect(loaded.rgb == img.rgb, "roundtrip bytes");
    }

    {
        const std::string path = temp_path("ascii.ppm");
        write_text(path, "P3\n2 1\n255\n255 0 0 0 255 0\n");
        auto img = pixelforge::load_ppm(path);
        expect(img.rgb[0] == 255 && img.rgb[3] == 0, "P3 load");
    }

    {
        bool threw = false;
        try {
            pixelforge::Image bad;
            bad.width = 2;
            bad.height = 1;
            bad.rgb = {1, 2, 3};
            pixelforge::validate_image(bad);
        } catch (const std::exception &) {
            threw = true;
        }
        expect(threw, "validate rejects size mismatch");
    }

    {
        bool threw = false;
        try {
            const std::string path = temp_path("trunc.ppm");
            write_text(path, "P3\n2 1\n255\n1 2\n");
            pixelforge::load_ppm(path);
        } catch (const std::exception &) {
            threw = true;
        }
        expect(threw, "truncated P3 throws");
    }

    if (fails) {
        std::cerr << fails << " assertion(s) failed\n";
        return 1;
    }
    std::cout << "test_image: ok\n";
    return 0;
}
