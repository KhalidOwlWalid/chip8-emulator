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

namespace chip8 {

    struct DecodedInstruction {
        std::uint16_t raw_instruction; // 16 bit
        std::uint8_t first_nibble; // 4 bit
        std::uint8_t VX; // 4 bit
        std::uint8_t VY; // 4 bit
        std::uint8_t N; // 4 bit
        std::uint8_t NN; // 8 bit
        std::uint16_t NNN; // 12 bit
    };

    constexpr DecodedInstruction parse(const std::uint16_t word) {
        return DecodedInstruction{
            .raw_instruction = static_cast<std::uint16_t>(word),
            .first_nibble = static_cast<std::uint8_t>((word & 0xF000) >> 12),
            .VX = static_cast<std::uint8_t>((word & 0x0F00) >> 8),
            .VY = static_cast<std::uint8_t>((word & 0x00F0) >> 4),
            .N = static_cast<std::uint8_t>((word & 0x000F)),
            .NN = static_cast<std::uint8_t>((word & 0x00FF)),
            .NNN = static_cast<std::uint16_t>((word & 0x0FFF))
        };
    }

    class Chip8 {

    public:
        Chip8(const std::filesystem::path rom_path);
        ~Chip8() = default;

        void fetch();
        void decode();
        void execute();
        bool has_rom_loaded();
        void disassemble();

        template<typename T>
        void print_bytes(const T& bytes);

    private:
        std::filesystem::path _rom_file_path;
        bool _rom_loaded = false;
        std::uint8_t _stack_count = 0;
        std::size_t _rom_start_address = 0x200;
        std::uint16_t _pc = _rom_start_address;
        std::uint16_t _curr_instruction;
        DecodedInstruction _decoded_instruction;

        std::array<uint8_t, 4096> _memory{};
        // The stack is only used to store return addresses when subroutines are called. The original RCA 1802 version allocated 48 bytes for up to 12 levels of nesting;
        // https://en.wikipedia.org/wiki/CHIP-8
        std::array<uint16_t, 24> _stack{};
        std::array<uint8_t, 16> _register{};

        std::vector<uint8_t> load_rom(const std::filesystem::path& p);
    };

}
