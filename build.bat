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

if /I "%~1"=="test" (
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude tests\test_image.cpp src\image.cpp -o build\test_image.exe
  if errorlevel 1 exit /b 1
  build\test_image.exe
  if errorlevel 1 exit /b 1
  echo Tests passed.
  goto :eof
)

echo Example: build\pixel-forge.exe --op gray --in samples\checker.ppm --out build\gray.ppm
echo Test:    build.bat test
endlocal
