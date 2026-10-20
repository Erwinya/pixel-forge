# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection

No external image libraries.

## Status

Complete: library, CLI, and build scripts (`Makefile`, `build.bat`).

## Build

```bash
make
./pixel-forge --op gray --in samples/checker.ppm --out gray.ppm
```

Windows (MinGW / LLVM):

```bat
build.bat
build\pixel-forge.exe --op gray --in samples\checker.ppm --out build\gray.ppm
```


## CLI options

| Flag | Default | Meaning |
|------|---------|---------|
| `--op` | required | `gray`, `blur`, or `edges` |
| `--in` | required | Input PPM path |
| `--out` | required | Output PPM path |
| `--radius` | `1` | Box-blur radius (blur only) |

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
