#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"

const char const *digits[9] = { "one", "two",   "three", "four", "five",
                                "six", "seven", "eight", "nine" };

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

    while (getline(&line, &len, fp) != -1) {

        bool found_num = false;
        bool found_second = false;
        u8 buf[2] = { 0 };

        for (u8 *c = line; *c != '\0'; c++) {

            if (isdigit(*c)) {
                *(buf + found_num) = *c;
                found_second = found_num;
                found_num = !found_num ? true : found_num;
                continue;
            }

            u8 text_buf[6] = { 0 };
            bool found_str = false;

            for (int i = 0; i < 5 && strlen(c) - i >= 0; i++) {
                *(text_buf + i) = *(c + i);

                if (isdigit(*(c + i)) && !found_num) {
                    *buf = *(c + i);
                    found_num = true;
                }

                for (int j = 0; j < 9; j++) {
                    if (strstr(text_buf, digits[j]) != NULL) {
                        // This is terribly awful and is evil to all that is good, but it is funny
                        // as it works.
                        *(buf + found_num) = j + 1 + '0';
                        found_second = found_num;
                        found_num = !found_num ? true : found_num;
                        found_str = true;
                        c += i - 1;
                        goto loop_end;
                    }
                }
            }
loop_end:
        }

        if (!found_second) {
            *(buf + 1) = *(buf);
        }

        sum += atoi(buf);
    }

    fprintf(stdout, "Sum: %ld\n", sum);

    fclose(fp);

    return 0;
}
