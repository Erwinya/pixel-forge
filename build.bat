@echo off
setlocal
where g++ >nul 2>&1
if errorlevel 1 (
  echo g++ not found. Install LLVM-MinGW or MSYS2, then re-run build.bat
  exit /b 1
)
if not exist build mkdir build
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src\main.cpp src\image.cpp -o build\pixel-forge.exe
if errorlevel 1 exit /b 1
echo Built build\pixel-forge.exe
echo Example: build\pixel-forge.exe --op gray --in samples\checker.ppm --out build\gray.ppm
endlocal
