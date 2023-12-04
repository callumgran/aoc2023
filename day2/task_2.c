#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../common.h"

#define RGB_OVERFLOW(rgb) (*(rgb) > 12 || *((rgb) + 1) > 13 || *((rgb) + 2) > 14)
GEN_MAP(rgb_index, u8, i32, lambda(u8, (u8 a, u8 b), { return a == b; }), 'r', 0, 'g', 1, 'b', 2);

int main(void)
{
    time_t t = clock();
    time_t start = clock();
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

        i32 id = str_next_i32(&c);

        i32 rgb[3] = { 0 };
        while (*c != '\0') {
            c += *c == ';';

            do {
                i32 tmp = str_next_i32(&c);
                if (tmp == -1)
                    goto loop_end;

                while (!isalpha(*c))
                    c++;

                SET_LARGEST_PRIMITIVE(rgb[rgb_index_map_get(*c)], tmp);

                while (isalpha(*c))
                    c++;
            } while (*c != ';' && *c != '\0');
        }

loop_end:

        box_sum += ARRAY_BINOP_RESULT(rgb, 3, *);
    }

    fprintf(stdout, "Sum: %ld\n", box_sum);

    fclose(fp);

    t = clock() - t;
    printf("Execution time: %fms\n", ((double)t) / CLOCKS_PER_SEC * 1000);

    return 0;
}
