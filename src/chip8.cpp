#include "../include/chip8.hpp"

using namespace chip8;

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

void Chip8::fetch() {
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

void Chip8::decode() {
    _decoded_instruction = chip8::parse(_curr_instruction);
}

void Chip8::execute() {
    // std::uint8_t instruction_nibble = instruction_bytes >> 12;
    // std::uint16_t rest_of_nibble = instruction_bytes & 0xFFF;
    // LOG_DEBUG("Instruction: {:X}, nibble: {:X}, rest of nibble: {:X}", instruction_bytes, instruction_nibble, rest_of_nibble);

    switch (_decoded_instruction.first_nibble) {

        case 0:
            LOG_DEBUG("Clear or return?");
            break;

        // Program jumps immediately to NNN without PC incrementing
        case 1:
            _pc = _decoded_instruction.NNN;
            LOG_DEBUG("PC: {:X}", _pc);
            break;

        // TODO: Push current PC to stack, and then jump to NNN
        // When subroutine reached 00EE, it returns by popping the last address from the stack and setting the PC to it
        // If multiple subroutines called, then this will recursively undo itself until it reached the first address pushed into the stack
        case 2:
            _stack[_stack_count] = _pc;
            _stack_count++;
            _pc = _decoded_instruction.NNN;
            LOG_DEBUG("Stack count: {}, Stack address: {:X}", _stack_count, spdlog::to_hex(_stack.begin(), _stack.end(), 2));
            break;

        // If VX == NN then skip one instruction
        case 3:
            if (_register[_decoded_instruction.VX] == _decoded_instruction.NN) {
                LOG_DEBUG("Instruction skipped because V{:X} == NN({})", _decoded_instruction.VX, _decoded_instruction.NN);
                _pc += 2;
            }
            break;

        // If VX != NN then skip one instruction
        case 4:
            LOG_DEBUG("Instruction skipped because V{:X} != NN({})", _decoded_instruction.VX, _decoded_instruction.NN);
            if (_register[_decoded_instruction.VX] != _decoded_instruction.NN) {
                _pc += 2;
            }
            break;

        // If VX == VY then skip one instruction
        case 5:
            LOG_DEBUG("Instruction skipped because V{:X} == V{:X}", _decoded_instruction.VX, _decoded_instruction.VY);
            if (_register[_decoded_instruction.VX] == _register[_decoded_instruction.VY]) {
                _pc += 2;
            }
            break;

        // If VX != VY then skip one instruction
        case 9:
            LOG_DEBUG("Instruction skipped because V{:X} != V{:X}", _decoded_instruction.VX, _decoded_instruction.VY);
            if (_register[_decoded_instruction.VX] != _register[_decoded_instruction.VY]) {
                _pc += 2;
            }

        // (6XNN): Set register X with NN
        case 6:
            LOG_DEBUG("Register V{:X} set with {}", _decoded_instruction.VX, _decoded_instruction.NN);
            _register[_decoded_instruction.VX] = _decoded_instruction.NN;
            break;

        // (7XNN): Add value NN to X
        // Does not set the carry flag at VF. Overflow is basically ignored.
        case 7:
            LOG_DEBUG("V{:X} += {}", _decoded_instruction.VX, _decoded_instruction.NN);
            _register[_decoded_instruction.VX] += _decoded_instruction.NN;
            break;

        case 8:

            switch (_decoded_instruction.N) {

                case 0:
                    LOG_DEBUG("V{:X} set to V{:X}", _register[_decoded_instruction.VX], _register[_decoded_instruction.VY]);
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VY];
                    break;

                case 1:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] | _register[_decoded_instruction.VY];
                    break;

                case 2:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] & _register[_decoded_instruction.VY];
                    break;

                case 3:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] ^ _register[_decoded_instruction.VY];
                    break;

                case 4:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] + _register[_decoded_instruction.VY];

                    if (_register[_decoded_instruction.VX] == 0xFF) {
                        // Raise the carry flag if the 2 registers addition overflows
                        _register[0xF] = 0x1;
                    } else {
                        _register[0xF] = 0;
                    }
                    break;

                case 5:
                    // This subtraction will also affect the carry flag, but note that it’s opposite from what you might think.
                    // If the minuend (the first operand) is larger than or equal to the subtrahend (second operand), VF will be set to 1.
                    // If the subtrahend is larger, and we “underflow” the result, VF is set to 0.
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] - _register[_decoded_instruction.VY];
                    if (_register[_decoded_instruction.VX] >= _register[_decoded_instruction.VY]) {
                        _register[0xF] = 1;
                    }
                    break;

                case 6:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VY];
                    // Get the LSB from the value in register X and store it in VF
                    _register[0xF] = (_register[_decoded_instruction.VX] & 0x1);
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] >> 1;
                    LOG_DEBUG("8XY6 instruction: Shifted right by 1 bit (<<) -> {:X} to {:X}", _register[_decoded_instruction.VY], _register[_decoded_instruction.VX]);
                    break;

                case 7:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VY] - _register[_decoded_instruction.VX];
                    if (_register[_decoded_instruction.VY] >= _register[_decoded_instruction.VX]) {
                        _register[0xF] = 1;
                    }
                    break;

                case 0xE:
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VY];
                    // Get the MSB bit from the value in register X and store it in VF
                    _register[0xF] = (_register[_decoded_instruction.VX] & 0x80);
                    _register[_decoded_instruction.VX] = _register[_decoded_instruction.VX] << 1;
                    LOG_DEBUG("8XYE instruction: Shifted left by 1 bit (<<) -> {:X} to {:X}", _register[_decoded_instruction.VY], _register[_decoded_instruction.VX]);
                    break;

                default:
                    break;
            }

        case 0xA:
            _idx_reg = _decoded_instruction.NNN;
            break;

        case 0xB:
            // WARNING: Ambiguous instruction. Different game .ch8 will either use the legacy chip8, chip-48 or super-chip
            // This may cause different interpretation behaviour. Hence, we should handle all implementation, and let the
            // user be able to configure which version they are using.
            LOG_WARN("BNNN not implemented");
            break;

        // TODO: (CXNN) generates a random number, binary ANDs it with the value in NN and put the result in VX
        case 0xC:
            LOG_WARN("CXNN not implemented");
            break;

        // TODO (DXYN) draws N pixel tall sprite at the horizontal X coordinate in VX and Y coordinate in VY
        // All pixels that are ON in the sprite will flip the pixel that is drawn on the screen. Any pixel turned OFF by this,
        // VF register gets set to 1. Otherwise, set to 0.
        case 0xD:
            LOG_WARN("DXYN not implemented");
            break;

        default:
            break;

    }

}
