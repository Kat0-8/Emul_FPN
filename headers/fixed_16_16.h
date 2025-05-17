#ifndef FIXED_16_16_H
#define FIXED_16_16_H
#include <stdint.h>
#include <math.h>
#include "utils.h"

typedef int32_t fixed_16_16_t;
#define SIGN_BIT_MASK_16_16 0x80000000 
#define FRACT_BITS_16_16 16
#define SCALE_FACTOR_16_16 (1<<FRACT_BITS_16_16) // = 2^16 = 65536
#define OVERFLOW_16_16 (fixed_16_16_t)0x80000000
#define FIXED_16_16_MIN -32768.0
#define FIXED_16_16_MAX 32767.99998474

fixed_16_16_t convert_to_fixed_16_16(double number);
double convert_from_fixed_16_16(fixed_16_16_t fixed_number);

fixed_16_16_t fixed_add_16_16(fixed_16_16_t fixed_addend_1, fixed_16_16_t fixed_addend_2);
fixed_16_16_t fixed_sub_16_16(fixed_16_16_t fixed_minend, fixed_16_16_t fixed_subtrahend);
fixed_16_16_t fixed_mul_16_16(fixed_16_16_t fixed_multiplicand, fixed_16_16_t fixed_multiplyer);
fixed_16_16_t fixed_div_16_16(fixed_16_16_t fixed_dividend, fixed_16_16_t fixed_divisor);

Operation_result ALF_fixed_16_16(Operations operation, fixed_16_16_t number_1, fixed_16_16_t number_2);

#endif