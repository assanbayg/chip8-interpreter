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

  void instr00E0();  // Clear screen

  void instr00EE();  // Popping the stack after 2NNN

  void instr1NNN(uint16_t NNN);  // Jump

  // Pushes the current PC to the stack
  // and sets PC to NNN
  void instr2NNN(uint16_t NNN);

  void instr3XNN(uint8_t X, uint8_t NN);

  void instr4XNN(uint8_t X, uint8_t NN);

  void instr5XY0(uint8_t X, uint8_t Y);

  void instr6XNN(uint8_t X, uint8_t NN);  // Set register VX

  void instr7XNN(uint8_t X, uint8_t NN);  // Add value to register VX

  // There is no actual 8XYN instruction in CHIP-8
  // However, I wanted to organize code without having multiple 8XY..
  // instructions
  void instr8XYN(uint8_t X, uint8_t Y, uint8_t N);

  void instr9XY0(uint8_t X, uint8_t Y);

  void instrANNN(uint16_t NNN);  // Set index register I

  void DXYN(uint8_t x_coord, uint8_t y_coord, uint8_t N);  // Display/Draw

 public:
  chip8() noexcept;

  uint8_t ram[MEMORY_SIZE];
  std::array<std::array<bool, WIDTH>, HEIGHT> display;

  uint16_t fetch();
  void decode(uint16_t opcode);

  ~chip8();
};

#endif