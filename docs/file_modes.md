# File modes

`std::ios::binary` tells a C++ stream to preserve the file's bytes instead of applying
text-mode translation. It does not interpret a file as a particular binary format.

- Unix and macOS usually use `LF` (`0x0A`).
- Windows usually uses `CRLF` (`0x0D 0x0A`).

On Windows, text mode may translate `CRLF` to `LF` while reading and `LF` to `CRLF`
while writing. This can change file sizes, offsets, headers, or binary asset data.

Use binary mode for images, audio, models, save files, and other data that must be read
byte for byte. Text mode is appropriate when reading human-readable text.

Older Windows text modes could also treat `Ctrl+Z` (`0x1A`) as end of file, another
historical reason to use binary mode for arbitrary data.

- [cppreference: `std::ios_base::openmode`](https://en.cppreference.com/cpp/io/ios_base/openmode)
- [Microsoft Learn: File Translation Constants](https://learn.microsoft.com/en-us/cpp/c-runtime-library/file-translation-constants)
