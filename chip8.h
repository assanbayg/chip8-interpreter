#ifndef CHIP8_
#define CHIP8_

#include <array>
#include <iostream>

class chip8 {
  // Memory
  static constexpr size_t MEMORY_SIZE = 4096;

  // Display constants
  static constexpr unsigned int WIDTH = 64;
  static constexpr unsigned int HEIGHT = 32;
  static constexpr unsigned int FREQUENCY = 60;

  uint8_t V[16];  // General purpose registers V0-VF

  uint16_t I;  // Index register

  uint16_t PC;  // Program counter

  uint16_t stack[16];  // Stack
  uint8_t sp;          // Stack pointer

  uint8_t delayTimer;
  uint8_t soundTimer;

  // Configuration for older games
  // Set to false for CHIP-48 and SUPER-CHIP
  bool USE_COSMAC_VIP_SHIFT = true;

  void instrDXYN(uint8_t x_coord, uint8_t y_coord, uint8_t N);  // Display/Draw

 public:
  uint8_t ram[MEMORY_SIZE];
  uint8_t key[16];
  std::array<std::array<bool, WIDTH>, HEIGHT> display;

  uint16_t fetch();
  void decode(uint16_t opcode);

  chip8() noexcept;
  ~chip8();
};

#endif