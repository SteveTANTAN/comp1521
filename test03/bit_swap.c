// swap pairs of bits of a 64-bit value, using bitwise operators

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// return value with pairs of bits swapped
uint64_t bit_swap(uint64_t value) {
    
    uint64_t temp = 0;
    
    for (int count = 0;count <= 63;count = count + 2){
        uint64_t num = 0x1;
        // if we wat it to be  bigger 1;
        // we only need to swap the middle two number;
        uint64_t L = value &(num << count);
        uint64_t U = value &(num << (count+1));

        temp |= L<<1;
        temp |= U>>1;
    }
    

    return temp;
}
