# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection
- binary threshold

No external image libraries.

## Status

Library, CLI, unit tests, and CI.

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

## Test

```bash
make test
```

```bat
build.bat test
```

## CLI options

| Flag | Default | Meaning |
|------|---------|---------|
| `--op` | required | `gray`, `blur`, `edges`, or `threshold` |
| `--in` | required | Input PPM path |
| `--out` | required | Output PPM path |
| `--radius` | `1` | Box-blur radius (`>= 0`) |
| `--threshold` | `128` | Cutoff for binary threshold (`0..255`) |

## Library

```cpp
#include "image.hpp"

auto img = pixelforge::load_ppm("samples/checker.ppm");
auto edges = pixelforge::sobel_edges(img);
auto mask = pixelforge::threshold(img, 128);
pixelforge::save_ppm("build/edges.ppm", edges);
```

Malformed buffers, truncated PPM files, and unsupported 16-bit P6 input raise exceptions.

## Requirements

- C++17 compiler

## License

MIT
