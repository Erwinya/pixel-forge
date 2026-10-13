# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection

No external image libraries.

## Status

Public image API (`include/image.hpp`) is in place. PPM IO, filters, CLI, and build scripts will land in follow-up commits.

## Library

```cpp
#include "image.hpp"

auto img = pixelforge::load_ppm("samples/checker.ppm");
auto gray = pixelforge::to_grayscale(img);
pixelforge::save_ppm("build/gray.ppm", gray);
```

## Planned usage

```powershell
.\build.bat
.\build\pixel-forge.exe --op gray --in samples\checker.ppm --out build\gray.ppm
```

## Requirements

- C++17 compiler

## License

MIT
