#include "bit_rotate.h"

// return the value bits rotated left n_rotations
uint16_t bit_rotate(int n_rotations, uint16_t bits) {
    uint32_t num = bits;
    n_rotations = n_rotations % 16;
    if (n_rotations < 0) {
        n_rotations += 16;
    }

    num <<= n_rotations;
    num = (num & 0XFFFF) |(num >> 16);
    return num;
}
