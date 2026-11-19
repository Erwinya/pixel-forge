#include "image.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int parse_nonneg(const std::string &text, const char *label) {
    try {
        size_t idx = 0;
        int value = std::stoi(text, &idx);
        if (idx != text.size() || value < 0) {
            throw std::runtime_error(std::string(label) + " must be a non-negative integer");
        }
        return value;
    } catch (const std::exception &) {
        throw std::runtime_error(std::string(label) + " must be a non-negative integer");
    }
}

int parse_threshold(const std::string &text) {
    int value = parse_nonneg(text, "threshold");
    if (value > 255) throw std::runtime_error("threshold must be 0..255");
    return value;
}

}  // namespace

int main(int argc, char **argv) {
    std::string op;
    std::string input;
    std::string output;
    int radius = 1;
    int cutoff = 128;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--op" && i + 1 < argc) {
                op = argv[++i];
            } else if (arg == "--in" && i + 1 < argc) {
                input = argv[++i];
            } else if (arg == "--out" && i + 1 < argc) {
                output = argv[++i];
            } else if (arg == "--radius" && i + 1 < argc) {
                radius = parse_nonneg(argv[++i], "radius");
            } else if (arg == "--threshold" && i + 1 < argc) {
                cutoff = parse_threshold(argv[++i]);
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "Usage: pixel-forge --op gray|blur|edges|threshold --in in.ppm --out out.ppm\n"
                          << "       [--radius N] [--threshold 0..255]\n";
                return 0;
            } else if (arg.rfind("--", 0) == 0) {
                std::cerr << "unknown argument: " << arg << '\n';
                return 2;
            } else {
                std::cerr << "unexpected argument: " << arg << '\n';
                return 2;
            }
        }

        if (op.empty() || input.empty() || output.empty()) {
            std::cerr << "missing required arguments\n";
            return 2;
        }

        pixelforge::Image img = pixelforge::load_ppm(input);
        pixelforge::Image out;
        if (op == "gray") {
            out = pixelforge::to_grayscale(img);
        } else if (op == "blur") {
            out = pixelforge::box_blur(img, radius);
        } else if (op == "edges") {
            out = pixelforge::sobel_edges(img);
        } else if (op == "threshold") {
            out = pixelforge::threshold(img, static_cast<uint8_t>(cutoff));
        } else {
            std::cerr << "unknown op: " << op << '\n';
            return 2;
        }
        pixelforge::save_ppm(output, out);
        std::cout << "wrote " << output << " (" << out.width << "x" << out.height << ")\n";
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }
}
