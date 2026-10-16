#include "image.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
    std::string op;
    std::string input;
    std::string output;
    int radius = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--op" && i + 1 < argc) {
            op = argv[++i];
        } else if (arg == "--in" && i + 1 < argc) {
            input = argv[++i];
        } else if (arg == "--out" && i + 1 < argc) {
            output = argv[++i];
        } else if (arg == "--radius" && i + 1 < argc) {
            radius = std::stoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: pixel-forge --op gray|blur|edges --in in.ppm --out out.ppm [--radius N]\n";
            return 0;
        }
    }

    if (op.empty() || input.empty() || output.empty()) {
        std::cerr << "missing required arguments\n";
        return 2;
    }

    try {
        pixelforge::Image img = pixelforge::load_ppm(input);
        pixelforge::Image out;
        if (op == "gray") {
            out = pixelforge::to_grayscale(img);
        } else if (op == "blur") {
            out = pixelforge::box_blur(img, radius);
        } else if (op == "edges") {
            out = pixelforge::sobel_edges(img);
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
