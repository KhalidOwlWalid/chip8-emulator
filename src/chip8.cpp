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
        _rom_file_path = rom_path;
        std::vector<std::uint8_t> rom_buf = load_rom(_rom_file_path);

        // TODO: Check the size of the loaded rom if it exceeds the expected size
        // If it does, this will cause an overflow
        std::copy(rom_buf.begin(), rom_buf.end(), _memory.begin() + _rom_start_address);

        LOG_INFO("Opening file: {} | Size: {} bytes", _rom_file_path.string(), _memory.size());
        _rom_loaded = true;
    }
}

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

void Chip8::disassemble() {
    // print out the loaded ROM in a readable fashion
    LOG_INFO("Loaded file: {}", _rom_file_path.string());
    LOG_INFO("dump: {:X}", spdlog::to_hex(_memory.begin(), _memory.end(), 2));
}

void Chip8::fetch_instruction() {
    // Read the current instruction that PC is CURRENTLY pointing at from memory
    // PC incremented by 2, reminder that PC is the address of the instructions
    // Basically, 0x202: 00EE
    std::uint8_t hi = _memory[_pc];
    std::uint8_t lo = _memory[_pc + 1];
    _pc += 2;

    _curr_instruction = static_cast<std::uint16_t>((hi << 8 | lo));
}

template <typename T>
void Chip8::print_bytes(const T& bytes) {
    LOG_DEBUG("Bytes debug: {:X}", bytes);
}

void Chip8::decode_and_execute(const std::uint16_t& instruction_bytes) {
    // Decode the current instruction

    // The first nibble will tell us what kind of instruction it is
    // The rest of the nibble (up to 12 bit), depends on the instruction nibble since there
    // are various options
    std::uint8_t instruction_nibble = instruction_bytes >> 12;
    std::uint16_t rest_of_nibble = instruction_bytes & 0xFFF;

    LOG_DEBUG("Instruction: {:X}, nibble: {:X}, rest of nibble: {:X}", instruction_bytes, instruction_nibble, rest_of_nibble);

}

void Chip8::decode_and_execute() {
    decode_and_execute(_curr_instruction);
}
