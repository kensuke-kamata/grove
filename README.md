# grove

A playground for game programming in C++.

## Build and Test

Requirements:

- CMake 3.28 or newer
- Git and network access to GitHub; CMake fetches raylib 6.0 during configuration
- A C++20 toolchain:
  - macOS: Xcode or Command Line Tools (Apple Clang and Make)
  - Windows: Visual Studio 2022 or Build Tools 2022 with the Desktop development with C++ workload (MSVC v143 and Windows SDK)

Run the following commands from the repository root.

On macOS:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/hello
./build/bin/raylib_window
ctest --test-dir build --output-on-failure
```

On Windows, use Developer PowerShell for Visual Studio 2022:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
.\build\bin\Debug\hello.exe
.\build\bin\Debug\raylib_window.exe
ctest --test-dir build -C Debug --output-on-failure
```

See [the build guide](docs/build/build.md) and [the testing guide](docs/testing/testing.md)
for more detail.

## CI

[GitHub Actions](.github/workflows/build.yml) builds and runs tests on macOS and Windows
in Debug and Release configurations.
