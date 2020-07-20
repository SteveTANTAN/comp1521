// Compare 2 floats using bit operations only

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"
#define b1s (bits1>>31)
#define b1e ((bits1>>23)&0xff)
#define b1f (bits1&0x7FFFFF)

#define b2s (bits2>>31)
#define b2e ((bits2>>23)&0xff)
#define b2f (bits2&0x7FFFFF)

// float_less is given the bits of 2 floats bits1, bits2 as a uint32_t
// and returns 1 if bits1 < bits2, 0 otherwise
// 0 is return if bits1 or bits2 is Nan
// only bit operations and integer comparisons are used
uint32_t float_less(uint32_t bits1, uint32_t bits2) {
    if ((((bits1 >> 23) ^ (0xFF)) == 0x0 
    && (bits1 & 0x7FFFFF) > 0x0
    )|| (((bits2 >> 23) ^ (0xFF)) == 0x0 
    && (bits2 & 0x7FFFFF) > 0x0)) {

        return 0;
    }

    if (b1f == 0x0 && b1f == b2f 
    && b1e == 0x0 && b1e == b2e) {
        return 0;
    }
    
    if (b1s == 1 && b2s == 0) {

        return 1;
    } 
    
    if (b1s == b2s && b1s == 0 && b1e < b2e) {
        return 1;
    }

    if (b1s == b2s && b1s == 0 
    && b1e == b2e && b1f < b2f) {
        return 1;
    }

    if (b1s == b2s && b1s == 1 && b1e > b2e) {
        return 1;
    }

    if (b1s == b2s && b1s == 1 
    && b1e == b2e && b1f > b2f) {
        return 1;
    }
    

    return 0;
}
