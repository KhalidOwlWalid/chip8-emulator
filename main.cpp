#include "logger.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdint>

#include <iostream>

std::vector<std::uint8_t> read_bytes(const std::filesystem::path& p) {
    std::ifstream in(p, std::ios::binary);
    if (!in) {
        // Couldnt open: handle it
    }

    uintmax_t size = std::filesystem::file_size(p);

    std::vector<std::uint8_t> buf(size);

    in.read(reinterpret_cast<char*>(buf.data()), static_cast<std::streamsize>(size));

    if (!in) {}
    return buf;
}

int main(int argc, char* argv[]){
    logger::init(spdlog::level::debug);
    LOG_INFO("chip8 emulator starting");

    // Try to read chip8 rom file
    std::filesystem::path rom_path {argv[1]};
    std::error_code ec;

    if (!std::filesystem::exists(rom_path, ec) || ec) {
        LOG_ERROR("File does not exists: {}", rom_path.string());
        return 1;
    } else {
        std::vector<std::uint8_t> rom_buf = read_bytes(rom_path);
        LOG_INFO("Opening file: {} | Size: {} bytes", rom_path.string(), rom_buf.size());

        for (size_t i = 0; i < rom_buf.size(); i++) {
            if (i % 16 == 0) {
                std::cout << '\n';
            }

            std::cout << rom_buf[i] << " ";
        }
    }

}
