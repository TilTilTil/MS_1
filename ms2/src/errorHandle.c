#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char getValidChar(const char *prompt, const char *validChars) {
    char input[16];

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            exit(1);
        }
        if (input[0] != '\n' && input[1] == '\n') {
            char choice = toupper((unsigned char)input[0]);
            if (strchr(validChars, choice) != NULL) {
                return choice;
            }
        }
        printf("Invalid input. Try again!");
    }
}

long getValidLongHelper() {
    char digit[64];  
    fgets(digit, sizeof(digit), stdin);
    long num = strtold(digit, NULL);
    return num;
}