// Swap bytes of a short

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// given uint16_t value return the value with its bytes swapped
uint16_t short_swap(uint16_t value) {
    uint16_t temp;
    temp = value & 0xff;
    temp = (temp << 8);
    temp |= (value >> 8);

    return temp;
}
