#ifndef FIXED_24_8_H
#define FIXED_24_8_H
#include <stdint.h>
#include <math.h>
#include "utils.h"

typedef int32_t fixed_24_8_t;
#define SIGN_BIT_MASK_24_8 0x80000000
#define FRACT_BITS_24_8 8
#define SCALE_FACTOR_24_8 (1<<FRACT_BITS_24_8) // = 2^8 = 256
#define OVERFLOW_24_8 (fixed_24_8_t)0x80000000
#define FIXED_24_8_MIN -8388608.0
#define FIXED_24_8_MAX 8388607.99609375

fixed_24_8_t convert_to_fixed_24_8(double number);
double convert_from_fixed_24_8(fixed_24_8_t fixed_number);

fixed_24_8_t fixed_add_24_8(fixed_24_8_t fixed_addend_1, fixed_24_8_t fixed_addend_2);
fixed_24_8_t fixed_sub_24_8(fixed_24_8_t fixed_minend, fixed_24_8_t fixed_subtrahend);
fixed_24_8_t fixed_mul_24_8(fixed_24_8_t fixed_multiplicand, fixed_24_8_t fixed_multiplyer);
fixed_24_8_t fixed_div_24_8(fixed_24_8_t fixed_dividend, fixed_24_8_t fixed_divisor);

Operation_result ALF_fixed_24_8(Operations operation, fixed_24_8_t number_1, fixed_24_8_t number_2);

#endif