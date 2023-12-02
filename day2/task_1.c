#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"

#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14

int main(void)
{
    FILE *fp = fopen("./day2/input.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    i64 id_sum = 0;

    while (getline(&line, &len, fp) != -1) {
        u8 *c = line;

        i32 id = str_next_int(&c);

        while (*c != '\0') {
            c += *c == ';';

            i64 red = 0;
            i64 green = 0;
            i64 blue = 0;
            while (*c != ';' && *c != '\0') {
                i32 tmp = str_next_int(&c);
                while (!isalpha(*c) && *c != '\0')
                    c++;

                if (*c == 'r') {
                    red += tmp;
                } else if (*c == 'g') {
                    green += tmp;
                } else if (*c == 'b') {
                    blue += tmp;
                }

                if (red > RED_MAX || green > GREEN_MAX || blue > BLUE_MAX) {
                    id = 0;
                    goto loop_end;
                }

                while (*c != '\0' && isalpha(*c)) {
                    c++;
                }
            }
        }

loop_end:

        id_sum += id;
    }

    fprintf(stdout, "Sum: %ld\n", id_sum);

    fclose(fp);

    return 0;
}
