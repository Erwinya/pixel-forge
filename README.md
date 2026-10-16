# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection

No external image libraries.

## Status

Library and CLI entrypoint are in place. Makefile / `build.bat` will land in a follow-up commit.

## Build (manual)

```powershell
g++ -std=c++17 -I include -o pixel-forge.exe src\image.cpp src\main.cpp
.\pixel-forge.exe --op gray --in samples\checker.ppm --out build\gray.ppm
```

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
