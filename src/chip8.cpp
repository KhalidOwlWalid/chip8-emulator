#include "../include/chip8.hpp"

Chip8::Chip8(const std::filesystem::path rom_path) {
    LOG_INFO("Chip8 interpretor initialized.");

    // Try to read chip8 rom file
    std::error_code ec;

    if (!std::filesystem::exists(rom_path, ec) || ec) {
        LOG_ERROR("File does not exists: {}", rom_path.string());
        _rom_loaded = false;
    } else {
        // ROM is parsed from the .ch8 file
        std::vector<std::uint8_t> rom_buf = load_rom(rom_path);

        // TODO: Check the size of the loaded rom if it exceeds the expected size
        // If it does, this will cause an overflow
        std::copy(rom_buf.begin(), rom_buf.end(), _memory.begin() + _rom_start_address);

        LOG_INFO("Opening file: {} | Size: {} bytes", rom_path.string(), _memory.size());
        LOG_INFO("dump: {:X}", spdlog::to_hex(_memory.begin(), _memory.end(), 1));
        _rom_loaded = true;
    }
}

// TODO: Change this to a reasonable array size, that matches the maximum ROM size
std::vector<std::uint8_t> Chip8::load_rom(const std::filesystem::path& p) {
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

bool Chip8::has_rom_loaded() {
    return _rom_loaded;
}

void Chip8::fetch() {
    // Read the current instruction that PC is CURRENTLY pointing at from memory
    // PC incremented by 2, reminder that PC is the address of the instructions
    // Basically, 0x202: 00EE

    std::uint8_t hi = _memory[_pc];
    std::uint8_t lo = _memory[_pc + 1];

    std::uint16_t instruction = static_cast<std::uint16_t>((hi << 8 | lo));

    LOG_INFO("Current instruction: {:x}", instruction);

    _pc += 2;

}

void Chip8::decode() {

}

void Chip8::execute() {

}

void Chip8::output() {
    LOG_DEBUG("{:x}", _pc);
}
