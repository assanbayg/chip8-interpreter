#include "chip8.h"

#include <cstring>
#include <random>

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

void chip8::instrDXYN(uint8_t x_coord, uint8_t y_coord, uint8_t N) {
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
      display{},
      key{} {
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
        std::memset(&display, 0, sizeof(display));
      else if (opcode == 0x00EE) {
        PC = stack[--sp];
      }
      break;
    case 0x1:  // 1NNN
      PC = NNN;
      break;
    case 0x2:  // 2NNN
      if (sp >= 16) {
        return;
      }

      stack[sp++] = PC;
      PC = NNN;
      break;
    case 0x3:  // 3XNN
      if (V[X] == NN) {
        PC += 2;
      }
      break;
    case 0x4:  // 4XNN
      if (V[X] != NN) {
        PC += 2;
      }
      break;
    case 0x5:  // 5XY0
      if (V[X] == V[Y]) {
        PC += 2;
      }
      break;
    case 0x6:  // 6XNN
      V[X] = NN;
      break;
    case 0x7:  // 7XNN
      V[X] += NN;
      break;
    case 0x8:
      switch (N) {
        case 0x0:
          V[X] = V[Y];
          break;
        case 0x1:
          V[X] = V[X] | V[Y];
          break;
        case 0x2:
          V[X] = V[X] & V[Y];
          break;
        case 0x3:
          V[X] = V[X] ^ V[Y];
          break;
        case 0x4:
          V[0xF] = (V[X] + V[Y]) > 255;
          V[X] += V[Y];
          break;
        case 0x5:
          V[0xF] = V[X] >= V[Y];
          V[X] -= V[Y];
          break;
        case 0x6:
          if (USE_COSMAC_VIP_SHIFT) {
            V[X] = V[Y];
          }
          V[0xF] = V[X] & 0x1;
          V[X] >>= 1;
          break;
        case 0x7:
          V[0xF] = V[Y] >= V[X];
          V[X] = V[Y] - V[X];
          break;
        case 0xE:
          if (USE_COSMAC_VIP_SHIFT) {
            V[X] = V[Y];
          }
          V[0xF] = (V[X] >> 7) & 0x1;
          V[X] <<= 1;
          break;
        default:
          break;
      }
      break;
    case 0x9:  // 9XY0
      if (V[X] != V[Y]) {
        PC += 2;
      }
      break;
    case 0xA:  // ANNN
      I = NNN;
      break;
    case 0xB:  // BNNN
      if (USE_COSMAC_VIP_SHIFT) {
        PC = NNN + V[0x0];
      } else {
        uint8_t X = (NNN >> 8) & 0x0F;
        PC = NNN + V[X];
      }
      break;
    case 0xC:  // CXNN
      static std::random_device rd;
      static std::mt19937 gen(rd());
      static std::uniform_int_distribution<uint16_t> distr(0, 255);
      uint8_t random_byte = static_cast<uint8_t>(distr(gen));
      V[X] = random_byte & NN;
      break;
    case 0xD:
      instrDXYN(V[X], V[Y], N);
      break;
    case 0xE:
      if (NN == 0x9E) {
        if (key[V[X]]) {  // 0xEX9E
          PC += 2;
        }
      } else if (NN == 0xA1) {
        if (!key[V[X]]) {  // 0xEX21
          PC += 2;
        }
      }
      break;
    case 0xF:
      switch (NN) {
        case 0x07:  // FX07
          V[X] = delayTimer;
          break;
        case 0x15:  // FX15
          delayTimer = V[X];
          break;
        case 0x18:  // FX18
          soundTimer = V[X];
          break;
        case 0x1E:  // FX1E
          I += V[X];
          break;
        case 0x0A:  // FX0A
          for (uint8_t i = 0; i < 16; ++i) {
            if (key[i]) {
              V[X] = i;
              return;
            }
          }
          PC -= 2;
          break;
        case 0x29:  // FX29
          I = 0x050 + (V[X] * 5);
          break;
        case 0x33:  // FX33
        {
          uint8_t n = V[X];
          ram[I] = n / 100;
          ram[I + 1] = n % 100 / 10;
          ram[I + 2] = n % 10;

          break;
        }
        case 0x55:  // FX55
          for (uint8_t i = 0; i <= X; ++i) {
            ram[I + i] = V[i];
          }
          break;
        case 0x65:  // FX65
          for (uint8_t i = 0; i < X; ++i) {
            V[I + i] = ram[i];
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

chip8::~chip8() {}