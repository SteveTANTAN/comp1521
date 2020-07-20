// Extract the 3 parts of a float using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

// separate out the 3 components of a float
float_components_t float_bits(uint32_t f) {
    float_components_t *node = malloc(sizeof(struct float_components));
    node->sign = f >> 31;
    node->exponent = (f >> 23) & (0xFF);
    node->fraction = (f & 0x7FFFFF);

    return *node;
}

// given the 3 components of a float
// return 1 if it is NaN, 0 otherwise
int is_nan(float_components_t f) {
    int temp = 0;
    if (((f.exponent) ^ 0xff) ==0 
    && ((f.fraction) != 0)) {
        temp = 1;
    }

    return temp;
}

// given the 3 components of a float
// return 1 if it is inf, 0 otherwise
int is_positive_infinity(float_components_t f) {
    int temp = 0;
    if (((f.exponent) ^ 0xff) ==0 
    && ((f.fraction) == 0) && (f.sign == 0)) {
        temp = 1;
    }

    return temp;
}

// given the 3 components of a float
// return 1 if it is -inf, 0 otherwise
int is_negative_infinity(float_components_t f) {
    int temp = 0;
    if (((f.exponent) ^ 0xff) ==0 
    && ((f.fraction) == 0) && (f.sign == 1)) {
        temp = 1;
    }

    return temp;
}

// given the 3 components of a float
// return 1 if it is 0 or -0, 0 otherwise
int is_zero(float_components_t f) {
    int temp = 0;
    if (f.exponent ==0 
    && ((f.fraction) == 0))  {
        temp = 1;
    }

    return temp;
}
