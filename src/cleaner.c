#include "cleaner.h"
#include <ctype.h>

void cleanInput(char *input)
{
    int i = 0;
    int j = 0;

    while (input[i] != '\0') {

        if (!isspace((unsigned char)input[i])) {
            input[j] = tolower((unsigned char)input[i]);
            j++;
        }

        i++;
    }

    input[j] = '\0';
}

