#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"

struct number_t {
    i32 num;
    i32 start;
    i32 end;
    i32 line;
};

struct gear_t {
    i32 pos;
    i32 line;
};

int main(void)
{
    FILE *fp = fopen("./day3/input.txt", "r");
    VECTOR(struct number_t, num);
    VECTOR(struct gear_t, gear);
    struct num_vector_t numbers;
    struct gear_vector_t gears;
    vector_init(&numbers, HR_GLOBAL_ALLOCATOR, 10000, NULL);
    vector_init(&gears, HR_GLOBAL_ALLOCATOR, 10000, NULL);

    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    i32 line_count = 1;

    while (getline(&line, &len, fp) != -1) {
        u8 *c_int = line;
        i32 start = 0;
        i32 end = 0;
        i32 prev_end = 0;
        i32 num = 0;
        while ((num = str_next_i32_0(&c_int, &start, &end)) != -1) {
            vector_push(&numbers, (&(struct number_t){ num, start + prev_end + 1,
                                                       end + prev_end + 1, line_count }));
            prev_end += end + 1;
        }

        u8 *c_sym = line;
        for (i32 i = 1; *c_sym != '\0' && *c_sym != '\n'; c_sym++, i++) {
            if (*c_sym == '*') {
                vector_push(&gears, &((struct gear_t){ i, line_count }));
            }
        }

        line_count++;
    }

    i64 sum = 0;
    vector_foreach(&gears, lambda(void, (const struct gear_t *const gear), {
        u64 tmp = 1;
        u8 num_counter = 0;
        for (i32 i = 0; i < vector_get_size(&numbers); i++) {
            struct number_t num = vector_get(&numbers, i);
            if (gear->line == num.line) {
                if (gear->pos == num.start - 1 || gear->pos == num.end + 1) {
                    tmp *= num.num;
                    num_counter++;
                }
            } else if (gear->line == num.line - 1 || gear->line == num.line + 1) {
                bool elem_within_left_range = num.start <= gear->pos && num.end >= gear->pos - 1;
                bool elem_within_right_range = num.start <= gear->pos + 1 && num.end >= gear->pos;
                if (elem_within_left_range || elem_within_right_range) {
                    tmp *= num.num;
                    num_counter++;
                }
            }
            if (num_counter > 2)
                break;
        }
        if (num_counter == 2) {
            sum += tmp;
        }
    }));

    fprintf(stdout, "Sum: %ld\n", sum);

    free(line);

    vector_free(&numbers);

    fclose(fp);

    return 0;
}
