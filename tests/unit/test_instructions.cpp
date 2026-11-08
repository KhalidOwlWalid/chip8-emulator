// Instruction tests for the CHIP-8 core.
//
// HOW IT WORKS
// Each test loads a few opcode bytes at 0x200, runs step() N times, and checks
// the machine state. The fixture below is the only place that knows about
// your Chip8 struct, so if you rename a field you change one line here,
// not every test.
//
// GETTING STARTED
// These tests are SKIPPED until you wire up the fixture:
//   1. Include your header and add your machine as a member (TODO 1).
//   2. Fill in the four helpers (TODO 2).
//   3. Set kFixtureWired = true.
// Then build and run:  ctest --output-on-failure   (from the build dir)
// Or one test only:    ./tests/test_main --gtest_filter='Instructions.Jump*'
//
// WRITING YOUR OWN TEST
//   TEST_F(Instructions, Something_Describes_Behaviour) {
//       load({0xAB, 0xCD});           // opcode bytes, as they sit in memory
//       step();
//       EXPECT_EQ(V(0x1), 0x42);      // EXPECT_ keeps going on failure, ASSERT_ stops
//   }
// Name the expected behaviour in the test name. A failure message then tells
// you what broke without you having to open the test.

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

// TODO 1: #include "chip8.hpp"

namespace {

constexpr bool kFixtureWired = false;  // TODO 3: flip to true once wired up
constexpr uint16_t kProgramStart = 0x200;

class Instructions : public ::testing::Test {
protected:
    // TODO 1: your machine, e.g.  Chip8 cpu;

    void SetUp() override {
        if (!kFixtureWired) {
            GTEST_SKIP() << "Fixture not wired to your Chip8 yet: see the "
                            "TODOs at the top of test_instructions.cpp";
        }
    }

    // TODO 2: fill these in for your struct.

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
    EXPECT_EQ(pc(), kProgramStart + 2) << "PC should move to the next instruction";
}

// ---- 7XNN: Vx += NN -------------------------------------------------------
TEST_F(Instructions, AddImmediate_7XNN_AddsNNToVx) {
    load({0x65, 0x10,    // V5 = 0x10
          0x75, 0x22});  // V5 += 0x22
    step();
    step();
    EXPECT_EQ(V(0x5), 0x32);
    EXPECT_EQ(pc(), kProgramStart + 4);
}

}  // namespace
