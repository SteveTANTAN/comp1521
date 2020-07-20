// Multiply a float by 2048 using bit operations only
// Steve tan
// z5237560
// 16/6/2020

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

float_components_t float_to_com(uint32_t f);
uint32_t float_from_com(float_components_t f);


// float_2048 is given the bits of a float f as a uint32_t
// it uses bit operations and + to calculate f * 2048
// and returns the bits of this value as a uint32_t
//
// if the result is too large to be represented as a float +inf or -inf is returned
//
// if f is +0, -0, +inf or -int, or Nan it is returned unchanged
//
// float_2048 assumes f is not a denormal number
//
/// For the `float_bits' exercise:

uint32_t float_2048(uint32_t f) {
    float_components_t node = float_to_com(f);
    if(node.exponent != 0 && node.exponent != 0xFF) {
    // excluding all the suff like zero or non.
    node.exponent += 11;
        if (node.exponent >= 0xff) {
        //if it is quit large, turing it to infinity,    
            node.fraction = 0;
            node.exponent = 0xff;
        }
        

    } else {

        return f;
    }

   return float_from_com(node);
}



float_components_t float_to_com(uint32_t f) {
    float_components_t node;
    node.sign = f >> 31;
    node.exponent = (f >> 23) & (0xFF);
    node.fraction = (f & 0x7FFFFF);
    return node;

}

uint32_t float_from_com(float_components_t f) {
return(f.sign << 31|    
        f.exponent << 23|
        f.fraction);

}