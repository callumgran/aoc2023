#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"

int main(void)
{
    FILE *fp = fopen("./day2/input.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    i64 mult_sum = 0;

    while (getline(&line, &len, fp) != -1) {
        u8 *c = line;

        i32 id = str_next_int(&c);

        i64 red = 0;
        i64 green = 0;
        i64 blue = 0;
        while (*c != '\0') {
            c += *c == ';';

            while (*c != ';' && *c != '\0') {
                i32 tmp = str_next_int(&c);
                while (!isalpha(*c) && *c != '\0')
                    c++;

                if (*c == 'r') {
                    if (tmp > red) {
                        red = tmp;
                    }
                } else if (*c == 'g') {
                    if (tmp > green) {
                        green = tmp;
                    }
                } else if (*c == 'b') {
                    if (tmp > blue) {
                        blue = tmp;
                    }
                }

                while (*c != '\0' && isalpha(*c)) {
                    c++;
                }
            }
        }

loop_end:

        mult_sum += red * green * blue;
    }

    fprintf(stdout, "Sum: %ld\n", mult_sum);

    fclose(fp);

    return 0;
}
