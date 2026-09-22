# Testing guide

The `hello` smoke test checks that the program exits successfully and prints
`Hello, grove!`. No external testing library is required.

On macOS:

```sh
ctest --test-dir build --output-on-failure --no-tests=error
```

On Windows:

```powershell
ctest --test-dir build -C Debug --output-on-failure --no-tests=error
```

For Release, use `--test-dir build-release` on macOS or `-C Release` on Windows.

Register additional non-interactive experiments in `tests/CMakeLists.txt` with
`add_test(NAME <name> COMMAND <target>)`. Only register programs that terminate on their own.
