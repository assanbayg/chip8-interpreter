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

void chip8::instr00E0() { std::memset(&display, 0, sizeof(display)); }

void chip8::instr00EE() { PC = stack[--sp]; }

void chip8::instr1NNN(uint16_t NNN) { PC = NNN; }

void chip8::instr2NNN(uint16_t NNN) {
  if (sp >= 16) {
    return;
  }

  stack[sp++] = PC;
  if (sp == 0) {
    return;
  }
  PC = NNN;
}

void chip8::instr3XNN(uint8_t X, uint8_t NN) {
  if (V[X] == NN) {
    PC += 2;
  }
}

void chip8::instr4XNN(uint8_t X, uint8_t NN) {
  if (V[X] != NN) {
    PC += 2;
  }
}

void chip8::instr5XY0(uint8_t X, uint8_t Y) {
  if (V[X] == V[Y]) {
    PC += 2;
  }
}

void chip8::instr6XNN(uint8_t X, uint8_t NN) { V[X] = NN; }

void chip8::instr7XNN(uint8_t X, uint8_t NN) { V[X] += NN; }

void chip8::instr9XY0(uint8_t X, uint8_t Y) {
  if (V[X] != V[Y]) {
    PC += 2;
  }
}

void chip8::instrANNN(uint16_t NNN) { I = NNN; }

void chip8::DXYN(uint8_t x_coord, uint8_t y_coord, uint8_t N) {
  x_coord %= 64;
  y_coord %= 32;
  V[0xF] = 0;

  for (uint8_t yline = 0; yline < N; ++yline) {
    if (y_coord + yline >= 32) break;  // Clip at the bottom

    uint8_t sprite_byte = ram[I + yline];

    for (uint8_t xline = 0; xline < 8; ++xline) {
      if (x_coord + xline >= 64) break;  // Clip at the right edge

      uint8_t sprite_pixel = (sprite_byte >> (7 - xline)) & 0x1;

      if (sprite_pixel) {
        if (display[y_coord + yline][x_coord + xline] == 1) {
          V[0xF] = 1;  // Collision
        }
        // Flipping the value
        display[y_coord + yline][x_coord + xline] ^= 1;
      }
    }
  }
}

chip8::chip8() noexcept
    : PC(0x200),
      ram{},
      V{},
      stack{},
      I(0),
      sp(0),
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
      if (opcode == 0x00E0)
        instr00E0();
      else {
        instr00EE();
      }
      break;
    case 0x1:
      instr1NNN(NNN);
      break;
    case 0x2:
      instr2NNN(NNN);
      break;
    case 0x3:
      break;
    case 0x4:
      break;
    case 0x5:
      break;
    case 0x6:
      instr6XNN(X, NN);
      break;
    case 0x7:
      instr7XNN(X, NN);
      break;
    case 0x8:
      break;
    case 0x9:
      break;
    case 0xA:
      instrANNN(NNN);
      break;
    case 0xB:
      break;
    case 0xC:
      break;
    case 0xD:
      DXYN(V[X], V[Y], N);
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