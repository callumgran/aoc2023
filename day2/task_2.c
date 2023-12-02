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

    i64 box_sum = 0;

    while (getline(&line, &len, fp) != -1) {
        u8 *c = line;

        i32 id = str_next_int(&c);

        i64 red = 0;
        i64 green = 0;
        i64 blue = 0;
        while (*c != '\0') {
            c += *c == ';';

            do {
                i32 tmp = str_next_int(&c);
                if (tmp == -1)
                    goto loop_end;

                while (!isalpha(*c))
                    c++;

                red = tmp > red && *c == 'r' ? tmp : red;
                green = tmp > green && *c == 'g' ? tmp : green;
                blue = tmp > blue && *c == 'b' ? tmp : blue;

                while (isalpha(*c))
                    c++;
            } while (*c != ';' && *c != '\0');
        }

loop_end:

        box_sum += red * green * blue;
    }

    fprintf(stdout, "Sum: %ld\n", box_sum);

    fclose(fp);

    return 0;
}
