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
        u8 str_buf[6] = { 0 };
        i32 id = 0;
        i32 consumed = 0;

        sscanf(c, "%s %d%n", str_buf, &id, &consumed);
        c += consumed + 1; // + 1 is to skip the colon

        i64 rgb[3] = { 0 };
        i32 tmp = 0;
        while (sscanf(c, "%d %s%n", &tmp, str_buf, &consumed) == 2) {
            c += consumed;

            rgb[rgb_index_map_get(*str_buf)] += tmp;

            if (RGB_OVERFLOW(rgb)) {
                id = 0;
                break;
            }

            if (*(str_buf + strlen(str_buf) - 1) == ';')
                memset(rgb, 0, sizeof(rgb));
        }
        id_sum += id;
    }

    fprintf(stdout, "Sum: %ld\n", id_sum);

    fclose(fp);
    free(line);

    t = clock() - t;
    printf("Execution time: %fms\n", ((double)t) / CLOCKS_PER_SEC * 1000);

    return 0;
}
