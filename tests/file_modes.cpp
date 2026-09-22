#include <fstream>
#include <iostream>

namespace {

std::string read_file(const char* path, std::ios_base::openmode mode) {
    std::ifstream file(path, mode);

    // alternative: construct string directly from stream-buffer iterators.
    //              this lets std::string manage its storage instead of using a pre-sized buffer.
    // return {
    //     std::istreambuf_iterator<char>(file),
    //     std::istreambuf_iterator<char>()
    // }

    file.seekg(0, std::ios::end);
    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buffer = std::make_unique<char[]>(static_cast<std::size_t>(size));
    file.read(buffer.get(), size);

    return std::string(buffer.get(), static_cast<std::size_t>(size));
}

void print_bytes(const std::string& data) {
    for (const unsigned char c : data) {
        std::cout << static_cast<int>(c) << " ";
    }
    std::cout << '\n';
}

}

int main() {
    constexpr char path[] = "file_modes.dat";

    {
        std::ofstream file(path, std::ios::binary);
        file.write("A\r\nB\r\n", 6);
    }

    const std::string text   = read_file(path, std::ios::in);
    const std::string binary = read_file(path, std::ios::in | std::ios::binary);

    std::cout << "text   size : " << text.size() << '\n';
    std::cout << "text   bytes: ";
    print_bytes(text);

    std::cout << "binary size : " << binary.size() << '\n';
    std::cout << "binary bytes: ";
    print_bytes(binary);

    std::remove(path);
    return 0;
}
