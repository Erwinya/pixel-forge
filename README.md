# pixel-forge

Minimal image processing toolkit in C++17 for **PPM** images:

- grayscale
- box blur
- Sobel edge detection

No external image libraries.

## Status

Project scaffolding is in place. Image IO, filters, CLI, and build scripts will land in follow-up commits.

## Planned usage

```powershell
.\build.bat
.\build\pixel-forge.exe --op gray --in samples\checker.ppm --out build\gray.ppm
```

## Requirements

- C++17 compiler

## License

MIT
