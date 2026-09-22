# Build guide

## Tools

On macOS, install Xcode or run `xcode-select --install` to install Command Line Tools.
Install CMake 3.28 or newer and make it available on PATH.
If you use Homebrew, run `brew install cmake`.

On Windows, install Visual Studio 2022 or Build Tools 2022 with the Desktop development
with C++ workload, MSVC v143, and Windows SDK. Make CMake 3.28 or newer available on PATH.
The CMake tools bundled with Visual Studio can be used if they meet this version requirement.

## Build configurations

See the [README](../../README.md) for Debug build and run commands.

On macOS, use a separate directory for Release:

```sh
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
./build-release/bin/hello
```

The Visual Studio generator supports multiple configurations in one build directory:

```powershell
cmake --build build --config Release
.\build\bin\Release\hello.exe
```

Use separate build directories when switching operating systems or generators.
Generated files belong under `build/`, `build-*/`, or `cmake-build-*/`, which are ignored by Git.

## Adding experiments

Place each experiment in `tests/<topic>.cpp` and add a target to
`tests/CMakeLists.txt`, for example:

```cmake
add_executable(memory memory.cpp)
```

Reconfigure and build the target:

```sh
cmake --build build --config Debug --target memory
```

Run `./build/bin/memory` on macOS or `.\build\bin\Debug\memory.exe` on Windows.
Target names must be unique. Keep each experiment's `main` in its own executable.
Shared implementation files belong in `src/` and public headers in `include/grove/`.
Define shared library targets in `src/CMakeLists.txt` when needed.

Write Markdown notes under `docs/` and link them from the README.
