#pragma once

#include "logger.hpp"
#include <filesystem>
#include <fstream>

class Chip8 {

public:
    Chip8(const std::filesystem::path rom_path);
    ~Chip8() = default;

    void fetch();
    void decode();
    void execute();
    void output();

    bool has_rom_loaded();

private:
    bool _rom_loaded = false;
    std::array<uint8_t, 4096> _memory{};
    std::size_t _rom_start_address = 0x200;
    std::uint16_t _pc = _rom_start_address;

    std::vector<uint8_t> load_rom(const std::filesystem::path& p);
};
