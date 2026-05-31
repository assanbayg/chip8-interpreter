#include "chip8.h"

#include <cstring>

static constexpr uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void chip8::instr00E0() {}

void chip8::instr1NNN() {}

void chip8::instr2NNN() {}

void chip8::instr00EE() {}

void chip8::instr6XNN() {}

void chip8::instr7XNN() {}

void chip8::instrANNN() {}

void chip8::DXYN() {}

chip8::chip8()
    : PC(0x200),
      ram{},
      V{},
      stack{},
      I(0),
      SP(0),
      delayTimer(0),
      soundTimer(0),
      display{} {
  // 0x050 is convenient start address for font data
  memcpy(ram + 0x050, font, sizeof(font));
}

uint16_t chip8::fetch() {
  // Combine two byte-instructions into one 16-bit instruction
  uint16_t opcode = (ram[PC] << 8) | ram[PC + 1];
  PC += 2;

  return opcode;
}

void chip8::decode(uint16_t opcode) {
  // D, X, Y, N -- 1..4th nibbles respectively
  uint8_t D = (opcode >> 12) & 0xF;
  uint8_t X = (opcode >> 8) & 0xF;
  uint8_t Y = (opcode >> 4) & 0xF;

  // Last 1..3 nibbles respectively
  uint8_t N = (opcode) & 0xF;
  uint8_t NN = (opcode) & 0xFF;
  uint16_t NNN = (opcode) & 0xFFF;

  // I know this is ugly...
  switch (D) {
    case 0x0:
      break;
    case 0x1:
      break;
    case 0x2:
      break;
    case 0x3:
      break;
    case 0x4:
      break;
    case 0x5:
      break;
    case 0x6:
      break;
    case 0x7:
      break;
    case 0x8:
      break;
    case 0x9:
      break;
    case 0xA:
      break;
    case 0xB:
      break;
    case 0xC:
      break;
    case 0xD:
      break;
    case 0xE:
      break;
    case 0xF:
      break;
    default:
      break;
  }
}

chip8::~chip8() {}