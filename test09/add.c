#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "add.h"

// return the MIPS opcode for add $d, $s, $t
uint32_t add(uint32_t d, uint32_t s, uint32_t t) {

    uint32_t new = 	0b00000000000000000000000000100000;
    new |= (d<<11);
    new |= (t<<16);
    new |= (s<<21);

    return new;
}
