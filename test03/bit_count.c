// count bits in a uint64_t

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// return how many 1 bits value contains
int bit_count(uint64_t value) {
    int number = 0;
    int temp = 0;
    uint64_t temp1 = 0x1;
    while ( number <= 63) {
        if ((value & (temp1 << number)) == (temp1 << number)) {
            temp++;
        }
        number ++;
    }
    return (temp);
}
