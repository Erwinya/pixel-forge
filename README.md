# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection

No external image libraries.

## Status

Public header, PPM load/save, grayscale, and a sample checker image are in place. Blur, Sobel edges, CLI, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "image.hpp"

auto img = pixelforge::load_ppm("samples/checker.ppm");
auto gray = pixelforge::to_grayscale(img);
pixelforge::save_ppm("build/gray.ppm", gray);
```

## Requirements

- C++17 compiler

## License

MIT
