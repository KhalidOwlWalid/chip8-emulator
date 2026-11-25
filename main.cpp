#include "logger.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdint>

#include "chip8.hpp"

#include <array>

#include <iostream>

int main(int argc, char* argv[]){
    logger::init(spdlog::level::debug);
    LOG_INFO("chip8 emulator starting");

    std::filesystem::path rom_path {argv[1]};

    Chip8 chip8 = Chip8(rom_path);

    if (chip8.has_rom_loaded()) {
        LOG_DEBUG("Chip8 ROM has been loaded");
    }

}
