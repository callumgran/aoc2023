#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"

int main(void)
{
    FILE *fp = fopen("./day1/input.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    i64 sum = 0;
    u8 buf[2] = { 0 };
    while (getline(&line, &len, fp) != -1) {
        bool found_num = false;
        bool found_second = false;
        memset(buf, 0, sizeof(buf));
        for (u8 *c = line; *c != '\0'; c++) {
            if (isdigit(*c)) {
                if (!found_num) {
                    buf[0] = *c;
                    found_num = true;
                } else {
                    buf[1] = *c;
                    found_second = true;
                }
            }
        }

        if (!found_second) {
            buf[1] = buf[0];
        }

        sum += atoi(buf);
    }

    fprintf(stdout, "Sum: %ld\n", sum);

    fclose(fp);

    return 0;
}
