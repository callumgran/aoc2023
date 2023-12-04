#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../common.h"

struct number_t {
    i32 num;
    i32 start;
    i32 end;
    i32 line;
};

struct symbol_t {
    u8 sym;
    i32 pos;
    i32 line;
};

int main(void)
{
    time_t t = clock();
    FILE *fp = fopen("./day3/input.txt", "r");
    VECTOR(struct number_t, num);
    VECTOR(struct symbol_t, sym);
    struct num_vector_t numbers;
    struct sym_vector_t symbols;
    vector_init(&numbers, HR_GLOBAL_ALLOCATOR, 5000, NULL);
    vector_init(&symbols, HR_GLOBAL_ALLOCATOR, 2500, NULL);

    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    i32 line_count = 0;

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
            if (!isdigit(*c_sym) && *c_sym != '.') {
                vector_push(&symbols, &((struct symbol_t){ *c_sym, i, line_count }));
            }
        }

        line_count++;
    }

    i64 sum = 0;

    vector_foreach(&numbers, lambda(void, (const struct number_t *const num), {
        for (i32 i = 0; i < vector_get_size(&symbols); i++) {
            struct symbol_t sym = vector_get(&symbols, i);
            if (sym.line == num->line) {
                if (sym.pos == num->start - 1 || sym.pos == num->end + 1) {
                    sum += num->num;
                    break;
                }
            } else if (sym.line == num->line - 1 || sym.line == num->line + 1) {
                bool elem_within_left_range = num->start <= sym.pos && num->end >= sym.pos - 1;
                bool elem_within_right_range = num->start <= sym.pos + 1 && num->end >= sym.pos;
                if (elem_within_left_range || elem_within_right_range) {
                    sum += num->num;
                    break;
                }
            }
        }
    }));

    fprintf(stdout, "Sum: %ld\n", sum);

    vector_free(&numbers);
    free(line);
    fclose(fp);

    t = clock() - t;
    printf("Execution time: %fms\n", ((double)t) / CLOCKS_PER_SEC * 1000);

    return 0;
}
