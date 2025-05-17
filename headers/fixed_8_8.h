#ifndef FIXED_8_8_H
#define FIXED_8_8_H
#include <stdint.h>
#include <math.h>
#include "utils.h"

typedef int16_t fixed_8_8_t;
#define SIGN_BIT_MASK_8_8 0x8000
#define FRACT_BITS_8_8 8
#define SCALE_FACTOR_8_8 (1<<FRACT_BITS_8_8) // = 2^8 = 256
#define OVERFLOW_8_8 (fixed_8_8_t)0x8000
#define FIXED_8_8_MAX 127.99609375
#define FIXED_8_8_MIN -128.0

fixed_8_8_t convert_to_fixed_8_8(double number);
double convert_from_fixed_8_8(fixed_8_8_t fixed_number);

fixed_8_8_t fixed_add_8_8(fixed_8_8_t fixed_addend_1, fixed_8_8_t fixed_addend_2);
fixed_8_8_t fixed_sub_8_8(fixed_8_8_t fixed_minend, fixed_8_8_t fixed_subtrahend);
fixed_8_8_t fixed_mul_8_8(fixed_8_8_t fixed_multiplicand, fixed_8_8_t fixed_multiplyer);
fixed_8_8_t fixed_div_8_8(fixed_8_8_t fixed_dividend, fixed_8_8_t fixed_divisor);

Operation_result ALF_fixed_8_8(Operations operation, fixed_8_8_t number_1, fixed_8_8_t number_2);
#endif