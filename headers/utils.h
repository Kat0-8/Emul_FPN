#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>

typedef enum { 
    ADDITION = 1, 
    SUBTRACTION, 
    MULTIPLICATION, 
    DIVISION, 
    CHANGE_PRECISION_FORMAT, 
    EXIT 
} Operations;
typedef enum { 
    EIGHT_EIGHT = 1, 
    SIXTEEN_SIXTEEN, 
    TWENTYFOUR_EIGHT, 
    RETURN 
} Precision_formats;
typedef enum {
    SUCCESS = 0,
    DIVISION_BY_ZERO,
    OVERFLOW,
    INVALID_INPUT
} Operation_statuses;

typedef struct {
    double number;
    Operation_statuses status;
} Operation_result;

void print_welcome_screen();
void print_operations_menu();
void print_change_precision_menu();
void print_operation_result(Operation_result* result, double number_1, double number_2, Operations operation);
void print_16_bits(uint16_t u_number);
void print_32_bits(uint32_t u_number);
void print_64_bits(uint64_t u_number);
void clear_screen_prompt();

int get_user_choice(int min, int max);
void get_two_double_numbers(double* number_1, double* number_2, Precision_formats current_precision_format);
const char* convert_operation_to_string(Operations operation);
const char* convert_precision_format_to_string(Precision_formats format);
const char* convert_operation_status_to_string(Operation_statuses status);
Operation_statuses check_overflow(double number, Precision_formats format);

void precision_change_handler(uint8_t* current_fractional_bits_precision, Precision_formats* current_precision_format);
void clear_screen();

Operation_result perform_operation(Operations operation, double number_1, double number_2, Precision_formats current_precision_format);

#endif