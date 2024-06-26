// under the MIT license, by marius851000
#include "rng.h"

// from https://lemire.me/blog/2019/07/03/a-fast-16-bit-random-number-generator/
uint16_t wyhash16_x; 

uint32_t hash16(uint32_t input, uint32_t key) {
  uint32_t hash = input * key;
  return ((hash >> 16) ^ hash) & 0xFFFF;
}

uint16_t wyhash16() {
  wyhash16_x += 0xfc15;
  return hash16(wyhash16_x, 0x2ab);
}