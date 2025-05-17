#include "utils.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fixed.h"

// ==================================================================

void print_welcome_screen() {
    printf("FIXED-POINT ARITHMETIC EMULATOR v1.0\n"
           "────────────────────────────────────────\n"
           "Welcome!\n"
           "This tool performs calculations using fixed-point arithmetic\n"
           "It can be used with numbers in formats: Q8.8, Q16.16, Q24.8\n");
}

void print_operations_menu() {
    printf("Available operations:\n"
        "[1] Addition\n"
        "[2] Subtraction\n"
        "[3] Multiplication\n"
        "[4] Division\n"
        "[5] Change precision format\n"
        "[6] Exit\n");
}

void print_change_precision_menu() {
    printf("Select precision format:\n"
           "[1] Q8.8\n"
           "[2] Q16.16\n"
           "[3] Q24.8\n"
           "[4] Return\n");
}

void print_operation_result(Operation_result* result, double number_1, double number_2, Operations operation) {
    switch(result->status) {
        case OVERFLOW: {
            const char* overflow_type;
            if(number_1 > 0 && number_2 > 0) {
                overflow_type = " positive";
            } else {
                if (number_1 < 0 && number_2 < 0) {
                    overflow_type = " negative";
                } else {
                    overflow_type = "";
                }
            }
            fprintf(stderr, "Error:%s %s occured!\n", overflow_type, convert_operation_status_to_string(result->status));
            break;
        }
        case DIVISION_BY_ZERO: {
            fprintf(stderr, "Error: invalid input. %s is forbidden!\n", convert_operation_status_to_string(result->status));
            break;
        }
        case SUCCESS: {
            printf("%s! Result of %s with %.9lf and %.9lf is %.9f\n", 
                convert_operation_status_to_string(result->status), 
                convert_operation_to_string(operation), 
                number_1, 
                number_2, 
                result->number);
            break;
        }
    }
}

void print_bit(int64_t bit) {
    if(bit) {
        putchar('1');
    } else {
        putchar('0');
    }
}

void print_16_bits(uint16_t u_number) {
    if((1<<15) & u_number) {
        putchar('1');
    } else {
        putchar('0');
    }
    putchar(' ');
    for (int i = 14; i>=8; i--) {
        uint16_t digit_mask = (uint16_t)1<<i;
        print_bit(digit_mask & u_number);
    }
    putchar(' ');
    for (int i = 7; i>=0; i--) {
        uint16_t digit_mask = (uint16_t)1<<i;
        print_bit(digit_mask & u_number);
    }
    putchar('\n');
}

void print_32_bits(uint32_t u_number) {
    if((1<<31) & u_number) {
        putchar('1');
    } else {
        putchar('0');
    }
    putchar(' ');
    for(int i = 30; i>=16; i--) {
        uint32_t digit_mask = (uint32_t)1<<i;
        print_bit(digit_mask & u_number);
    }
    putchar(' ');
    for(int i = 15; i>=0; i--) {
        uint32_t digit_mask = (uint32_t)1<<i;
        print_bit(digit_mask & u_number);
    }
    putchar('\n');
}

void print_64_bits(uint64_t u_number) {
    if((1ULL<<63) & u_number) {
        putchar('1');
    } else {
        putchar('0');
    }
    putchar(' ');
    for(int i=62; i>=32; i--) {
        uint64_t digit_mask = (uint64_t)1ULL<<i;
        print_bit(digit_mask & u_number);
    }
    putchar(' ');
    for(int i=31; i>=0; i--) {
        uint64_t digit_mask = (uint64_t)1ULL<<i;
        print_bit(digit_mask & u_number);
    }
    putchar('\n');
}

void clear_screen_prompt() {
    char response;
    while (getchar() != '\n');
    while (1) {
        printf("Do you want to clear the screen? (y/n): ");
        scanf("%c", &response);
        
        if (response == 'y' || response == 'Y') {
            clear_screen();
            break;
        }
        if (response == 'n' || response == 'N') {
            break;
        }
        fprintf(stderr, "Error: invalid input. Enter a y/Y or n/N!\n");
            while(getchar() != '\n');
            continue;
    }
}

//===================================================================

int get_user_choice(int min, int max) {
    int choice;
    int result;
    printf("Enter your choice: ");
    result = scanf("%d", &choice);

    if(result == EOF) {
        fprintf(stderr, "Error: Unexpected end of input.\n");
        exit(EXIT_FAILURE);
    }

    if(result == 1) {
        if (choice < min || choice > max) {
            fprintf(stderr, "Error: invalid choice. Enter a number in range [%d;%d]\n", min, max);
            return -1;
        }
    } else {
        int c;
        while ((c = getchar ()) != '\n' && c != EOF);
        fprintf(stderr, "Error: invalid input. Enter a number!\n");
        return -1;
    }
    return choice;
}

void get_two_double_numbers(double* number_1, double* number_2, Precision_formats current_precision_format) {
    
    while (1) {
        printf("Enter two numbers: ");
        int result = scanf("%lf %lf", number_1, number_2);
        
        if (result == EOF) {
            fprintf(stderr, "Error: Unexpected end of input.\n");
            exit(EXIT_FAILURE);
        }
        
        if (result == 2) {
            if (check_overflow(*number_1, current_precision_format) != OVERFLOW &&
                check_overflow(*number_2, current_precision_format) != OVERFLOW) {
                break;
            }
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fprintf(stderr, "Error: invalid input. Please enter two numbers.\n");
        }
    }
}

const char* convert_operation_to_string(Operations operation) {
    switch(operation) {
        case ADDITION: return "ADDITION";
        case SUBTRACTION: return "SUBTRACTION";
        case MULTIPLICATION: return "MULTIPLICATION";
        case DIVISION: return "DIVISION";
        default: return "UNKNOWN";
    }
}

const char* convert_precision_format_to_string(Precision_formats format) {
    switch(format) {
        case EIGHT_EIGHT: return "8.8";
        case SIXTEEN_SIXTEEN: return "16.16";
        case TWENTYFOUR_EIGHT: return "24.8";
        default: return "UNKNOWN";
    }
}

const char* convert_operation_status_to_string(Operation_statuses status) {
    switch(status){
        case SUCCESS : return "SUCCESS";
        case DIVISION_BY_ZERO : return "DIVISION BY ZERO";
        case OVERFLOW : return "OVERFLOW";
        case INVALID_INPUT : return "INVALID INPUT";
    }
}

Operation_statuses check_overflow(double number, Precision_formats format) {
    Operation_statuses is_overflow = SUCCESS;
    double range_max, range_min;
    switch(format) {
        case EIGHT_EIGHT: {
            if(number > FIXED_8_8_MAX || number < FIXED_8_8_MIN) {
                is_overflow = OVERFLOW;
                range_min = FIXED_8_8_MIN;
                range_max = FIXED_8_8_MAX;
            }
            break;
        }
        case TWENTYFOUR_EIGHT: {
            if(number > FIXED_24_8_MAX || number < FIXED_24_8_MIN) {
                is_overflow = OVERFLOW;
                range_min = FIXED_24_8_MIN;
                range_max = FIXED_24_8_MAX;
            }
        }
        case SIXTEEN_SIXTEEN: {
            if(number > FIXED_16_16_MAX || number < FIXED_16_16_MIN) {
                is_overflow = OVERFLOW;
                range_min = FIXED_16_16_MIN;
                range_max = FIXED_16_16_MAX;
            }
            break;
        }
    }
    if(is_overflow == OVERFLOW) {
        fprintf(stderr, "Overflow occured in number %lf => %s format range is [%lf;%lf]\n",
            number, convert_precision_format_to_string(format), range_min, range_max);
    }
    return is_overflow;
}

// ==================================================================

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void precision_change_handler(uint8_t* current_fractional_bits_precision, Precision_formats* current_precision_format) {
    int choice;
    print_change_precision_menu();

    while((choice = get_user_choice(EIGHT_EIGHT, RETURN)) == -1);

    switch(choice) {
        case EIGHT_EIGHT:
            *current_fractional_bits_precision = 8;
            *current_precision_format = EIGHT_EIGHT;
            break;
        case SIXTEEN_SIXTEEN:
            *current_fractional_bits_precision = 16;
            *current_precision_format = SIXTEEN_SIXTEEN;
            break;
        case TWENTYFOUR_EIGHT:
            *current_fractional_bits_precision = 8;
            *current_precision_format = TWENTYFOUR_EIGHT;
            break;
        case RETURN:
            break;
    }
}

Operation_result perform_operation(Operations operation, double number_1, double number_2, Precision_formats current_precision_format) {
    
    Operation_result result;
    printf("Starting the %s operation...\n", convert_operation_to_string(operation));
    switch(current_precision_format) {
        case EIGHT_EIGHT: {
            result = ALF_fixed_8_8(operation, convert_to_fixed_8_8(number_1), convert_to_fixed_8_8(number_2));
            break;
        }
        case SIXTEEN_SIXTEEN:{
            result = ALF_fixed_16_16(operation, convert_to_fixed_16_16(number_1), convert_to_fixed_16_16(number_2));
            break;
        }
        case TWENTYFOUR_EIGHT: {
            result = ALF_fixed_24_8(operation, convert_to_fixed_24_8(number_1), convert_to_fixed_24_8(number_2));
            break;
        }
    }
    return result;
}