#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include "chip8.hpp"

namespace {

class Instructions : public ::testing::Test {
protected:

    void SetUp() override {}

    // Put `bytes` into memory starting at 0x200 (where ROMs load).
    void load(const std::vector<uint8_t>& bytes) { (void)bytes; }

    // Execute exactly one instruction.
    void step() {}

    // Read register Vx (x = 0x0..0xF).
    uint8_t V(int x) { (void)x; return 0; }

    // Read the program counter.
    uint16_t pc() { return 0; }
};

// ---- 1NNN: jump to NNN ----------------------------------------------------
TEST_F(Instructions, Jump_1NNN_SetsPcToNNN) {
    load({0x13, 0x48});
    step();
    EXPECT_EQ(pc(), 0x348);
}

// ---- 6XNN: set Vx = NN ----------------------------------------------------
TEST_F(Instructions, SetRegister_6XNN_LoadsNNIntoVx) {
    load({0x63, 0x7C});
    step();
    EXPECT_EQ(V(0x3), 0x7C);
}

// ---- 7XNN: Vx += NN -------------------------------------------------------
TEST_F(Instructions, AddImmediate_7XNN_AddsNNToVx) {
    load({0x65, 0x10,    // V5 = 0x10
          0x75, 0x22});  // V5 += 0x22
    step();
    step();
    EXPECT_EQ(V(0x5), 0x32);
}

}  // namespace
