#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "fixed.h"

uint8_t current_fractional_bits_precision = 16;
Precision_formats current_precision_format = SIXTEEN_SIXTEEN;

int main() {
    int choice;
    double number_1, number_2;
    Operation_result result;

    print_welcome_screen();

    while (1) {
        printf("Current precision format: Q%s\n", convert_precision_format_to_string(current_precision_format));

        print_operations_menu();
        
        while ((choice = get_user_choice(ADDITION, EXIT)) == -1);

        if (choice == EXIT) {
            printf("Exiting...\n");
            break;
        }

        if (choice == CHANGE_PRECISION_FORMAT) {
            precision_change_handler(&current_fractional_bits_precision, &current_precision_format);
            printf("Your new precision format is Q%s\n\n", convert_precision_format_to_string(current_precision_format));
            continue;
        }

        get_two_double_numbers(&number_1, &number_2, current_precision_format);

        Operations operation = (Operations)choice;
        result = perform_operation(operation, number_1, number_2, current_precision_format);

        print_operation_result(&result, number_1, number_2, operation);

        clear_screen_prompt();
    }

    return 0;
}