#include "logger.hpp"

#include <filesystem>
#include "chip8.hpp"

int main(int argc, char* argv[]){
    logger::init(spdlog::level::debug);
    LOG_INFO("chip8 emulator starting");

    std::filesystem::path rom_path {argv[1]};

    Chip8 chip8 = Chip8(rom_path);

    if (chip8.has_rom_loaded()) {
        LOG_DEBUG("Chip8 ROM has been loaded");
    } else {
        LOG_ERROR("ROM failed to be loaded");
        return 0;
    }

    // chip8.disassemble();

    for (int i = 0; i < 20; i++) {
        chip8.fetch_instruction();
        chip8.decode_and_execute();
    }

    // while (true) {
    //     chip8.fetch();
    //     chip8.decode_and_execute();
    // }
}
