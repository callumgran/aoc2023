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
    u8 buf[2] = { 0 };
    u8 text_buf[6] = { 0 };
    u64 line_no = 0;
    while (getline(&line, &len, fp) != -1) {
        line_no++;
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

                continue;
            }

            memset(text_buf, 0, sizeof(text_buf));
            bool found_str = false;
            for (int i = 0; i < 5 && strlen(c) - i >= 0; i++) {
                text_buf[i] = *(c + i);
                if (isdigit(*(c + i)) && !found_num) {
                    buf[0] = *(c + i);
                    found_num = true;
                }
                for (int j = 0; j < 9; j++) {
                    if (strstr(text_buf, digits[j]) != NULL) {
                        if (!found_num) {
                            buf[0] = j + 1 + '0';
                            found_num = true;
                        } else {
                            buf[1] = j + 1 + '0';
                            found_second = true;
                        }
                        found_str = true;
                    }
                }

                if (found_str) {
                    c += i - 1;
                    break;
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
