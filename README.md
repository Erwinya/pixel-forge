# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection

No external image libraries.

## Status

Library filters are complete (PPM IO, grayscale, box blur, Sobel). CLI and build scripts will land in follow-up commits.

## Library

```cpp
#include "image.hpp"

auto img = pixelforge::load_ppm("samples/checker.ppm");
auto edges = pixelforge::sobel_edges(img);
pixelforge::save_ppm("build/edges.ppm", edges);
```

## Requirements

- C++17 compiler

## License

MIT
