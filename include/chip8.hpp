#pragma once

#include "logger.hpp"
#include <filesystem>
#include <fstream>

/* Chip8 Specifications
 *
 * Memory: 4kB RAM
 * Display size: 64 x 32 pixels
 * One 16-bit register called "I" -> Used to point at locations in memory
 * 16 8-bit (one byte) general purpose variable registers, V0 to VF
 * - VF typically used as carry flag
 *
 * Copied from https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
*/

class Chip8 {

public:
    Chip8(const std::filesystem::path rom_path);
    ~Chip8() = default;

    void fetch_instruction();
    void decode_and_execute();
    void decode_and_execute(const std::uint16_t& instruction_bytes);
    bool has_rom_loaded();
    void disassemble();

    template<typename T>
    void print_bytes(const T& bytes);

private:
    std::filesystem::path _rom_file_path;
    bool _rom_loaded = false;
    std::array<uint8_t, 4096> _memory{};
    std::size_t _rom_start_address = 0x200;
    std::uint16_t _pc = _rom_start_address;
    std::uint16_t _curr_instruction;

    std::vector<uint8_t> load_rom(const std::filesystem::path& p);
};
