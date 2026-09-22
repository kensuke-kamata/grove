#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

namespace {

struct read_result {
    std::size_t buffer_size;
    std::size_t read_size;
    std::string data;
};

read_result read_file(const char* path, std::ios_base::openmode mode) {
    std::ifstream file(path, mode);

    // NOTE: alternative string construction: std::string{istreambuf_iterator, end_iterator}
    //       std::string manages the resulting storage

    file.seekg(0, std::ios::end);
    const std::streamsize buffer_size = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buffer = std::make_unique<char[]>(static_cast<std::size_t>(buffer_size));
    file.read(buffer.get(), buffer_size);

    const std::streamsize read_size = file.gcount();
    return {
        static_cast<std::size_t>(buffer_size),
        static_cast<std::size_t>(read_size),
        std::string(buffer.get(), static_cast<std::size_t>(read_size))
    };
}

void print_bytes(const std::string& data) {
    for (const unsigned char byte : data) {
        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(byte)
                  << ' ';
    }
    std::cout << std::dec
              << std::setfill(' ')
              << '\n';
}

void print_result(const char* mode_name, const read_result& result) {
    std::cout << '[' << mode_name << " mode]\n"
              << "buffer size : " << result.buffer_size << '\n'
              << "read   size : " << result.read_size << '\n'
              << "data  (hex) : ";
    print_bytes(result.data);
    std::cout << '\n';
}

}

int main() {
    constexpr char path[] = "file_modes.dat";

    {
        std::ofstream file(path, std::ios::binary);
        file.write("A\r\nB\r\n", 6);
    }

    const read_result text   = read_file(path, std::ios::in);
    const read_result binary = read_file(path, std::ios::in | std::ios::binary);

    print_result("text", text);
    print_result("binary", binary);

    std::remove(path);
    return 0;
}
