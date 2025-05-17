#include "fixed_16_16.h"
#include <math.h>
#include "utils.h"
#include "stdio.h"
#include <stdbool.h>

fixed_16_16_t convert_to_fixed_16_16(double number) {
    return (fixed_16_16_t)round(number * SCALE_FACTOR_16_16);
}

double convert_from_fixed_16_16(fixed_16_16_t fixed_number) {
    return (double)((double)fixed_number / SCALE_FACTOR_16_16);
}

fixed_16_16_t fixed_add_16_16(fixed_16_16_t fixed_addend_1, fixed_16_16_t fixed_addend_2) {

    uint32_t a = fixed_addend_1;
    uint32_t b = fixed_addend_2;

    printf("First addend bits: \n");
    print_32_bits(a);
    printf("Second addend bits: \n");
    print_32_bits(b);

    uint32_t sum = a + b;

    printf("Summary bits: \n");
    print_32_bits(sum);

    if(!((a ^ b) & SIGN_BIT_MASK_16_16) && ((a ^ sum) & SIGN_BIT_MASK_16_16)) {
        return (fixed_16_16_t)OVERFLOW_16_16;
    }

    return (fixed_16_16_t)sum;   
}

fixed_16_16_t fixed_sub_16_16(fixed_16_16_t fixed_minuend, fixed_16_16_t fixed_subtrahend) {

    uint32_t a = fixed_minuend;
    uint32_t b = fixed_subtrahend;

    printf("Minuend bits: \n");
    print_32_bits(a);
    printf("Subtrahend bits: \n");
    print_32_bits(b);

    uint32_t b_twos_complement = ~b + 1;

    printf("Two's complement subtrahend bits: \n");
    print_32_bits(b_twos_complement);

    uint32_t diff = a + b_twos_complement;

    printf("Difference bits: \n");
    print_32_bits(diff);

    if(((a ^ b) & SIGN_BIT_MASK_16_16) && ((a ^ diff) & SIGN_BIT_MASK_16_16)) {
        return (fixed_16_16_t)OVERFLOW_16_16;
    }

    return (fixed_16_16_t)diff;
}

fixed_16_16_t fixed_mul_16_16(fixed_16_16_t fixed_multiplicand, fixed_16_16_t fixed_multiplyer) {

    int64_t product = 0;

    printf("Multiplicand bits: \n");
    print_32_bits((uint32_t)fixed_multiplicand);
    printf("Multiplyer bits: \n");
    print_32_bits((uint32_t)fixed_multiplyer);

    uint32_t a = (fixed_multiplicand < 0 ) ? -fixed_multiplicand : fixed_multiplicand;
    uint32_t b = (fixed_multiplyer < 0) ? -fixed_multiplyer : fixed_multiplyer;

    printf("Absolute Multiplicand bits: \n");
    print_32_bits(a);
    printf("Absolute Multiplyer bits: \n");
    print_32_bits(b);
    printf("\n");

    bool sign = (fixed_multiplicand ^ fixed_multiplyer) >> 31;

    for(int i = 0; i < 32; i++) {
        if(b & (1ULL << i)) {
            product += (int64_t)a << i;
        }
        printf("Iteration: %d\n", i+1);
        printf("Product bits: \n");
        print_64_bits((uint64_t)product);
        printf("\n");
    }

    if(sign) {
        product = -product;
    }

    uint32_t upper_bits = product >> (63 - FRACT_BITS_16_16);
    if((product < 0 && ~upper_bits) || (product >= 0 && upper_bits)) {
        return (fixed_16_16_t)OVERFLOW_16_16;
    } 

    product += (1LL << (FRACT_BITS_16_16 - 1));

    printf("Result product bits: \n");
    print_64_bits(product);
    printf("\n");

    return (fixed_16_16_t)(product >> FRACT_BITS_16_16);
}

fixed_16_16_t fixed_div_16_16(fixed_16_16_t fixed_dividend, fixed_16_16_t fixed_divisor) {

    printf("Dividend bits: \n");
    print_32_bits((uint32_t)fixed_dividend);
    printf("Divisor bits: \n");
    print_32_bits((uint32_t)fixed_divisor);

    uint32_t a = (fixed_dividend < 0) ? -fixed_dividend : fixed_dividend;
    uint32_t b = (fixed_divisor < 0) ? -fixed_divisor : fixed_divisor;
    uint32_t bit = 0x10000;
    int iter = 1;
    int adjust_iter = 1;
    bool sign = (fixed_dividend ^ fixed_divisor) >> 31;

    printf("Absolute Dividend/Remainder bits: \n");
    print_32_bits(a);
    printf("Absolute Divisor bits: \n");
    print_32_bits(b);
    printf("\n");

    while(b < a) {
        printf("Adjusting divisor iteration: %d\n", adjust_iter++);
        b <<= 1;
        printf("Divisor bits: \n");
        print_32_bits(b);
        printf("\n");
        bit <<= 1;
    }

    if(!bit) {
        return (fixed_16_16_t)OVERFLOW_16_16;
    }

    uint32_t quotient = 0;

    if(b & SIGN_BIT_MASK_16_16) {
        printf("Division iteration: %d\n", iter++);
        if(a>=b) {
            quotient |= bit;
            a -= b;
        }
        printf("Quotient bits: \n");
        print_32_bits(quotient);
        printf("Remainder bits: \n");
        print_32_bits(a);
        printf("Shifting >> divisor to avoid overflow: \n");
        print_32_bits(b);
        b >>=1;
        print_32_bits(b);
        printf("\n");
        bit >>= 1;
    }

    while(bit && a) {
        printf("Division iteration: %d\n", iter++);
        if(a >= b) {
            quotient |= bit;
            a -= b;
            printf("Quotient bits: \n");
            print_32_bits(quotient);
            printf("Remainder bits: \n");
            print_32_bits(a);
        }
        printf("Adjusting remainder to continue division: \n");
        print_32_bits(a);
        a <<= 1;
        print_32_bits(a);
        printf("\n");
        bit >>= 1;
    }

    if(a>=b){
        quotient++;
    }

    if(sign) {
        quotient = -quotient;
    }

    printf("Result quotient bits:\n");
    print_32_bits(quotient);

    return (fixed_16_16_t)(quotient);
}

Operation_result ALF_fixed_16_16(Operations operation, fixed_16_16_t number_1, fixed_16_16_t number_2) {

    Operation_result result;
    result.status = SUCCESS;
    fixed_16_16_t number;
    switch(operation) {
        case ADDITION: {
            number = fixed_add_16_16(number_1, number_2);
            break;
        }
        case SUBTRACTION: {
            number = fixed_sub_16_16(number_1, number_2);
            break;
        }
        case MULTIPLICATION: {
            number = fixed_mul_16_16(number_1, number_2);
            break;
        }
        case DIVISION: {
            if (number_2 == 0) {
                result.status = DIVISION_BY_ZERO;
                break;
            }
            number = fixed_div_16_16(number_1, number_2);
            break;
        }
    }
    if (number == OVERFLOW_16_16) {
        result.status = OVERFLOW;
        return result;
    }
    result.number = convert_from_fixed_16_16(number);
    return result;
}