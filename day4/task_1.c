#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"

int main(void)
{
    FILE *fp = fopen("./day4/input.txt", "r");
    SHASHSET(i32, i32);
    struct i32_shashset_t winning_nums;
    shashset_init(&winning_nums, HR_GLOBAL_ALLOCATOR, 12, cmp_i32, hash_int);

    if (fp == NULL)
        return 1;

    char *line = NULL;
    size_t len = 0;

    u64 sum = 0;
    while (getline(&line, &len, fp) != -1) {
        u8 *c = line;
        str_next_i32_0(&c, &(int){0}, &(int){0});

        i32 num = 0;
        while (*(c + 1) != '|' && (num = str_next_i32_0(&c, &(int){0}, &(int){0})) != -1)
            shashset_insert(&winning_nums, num);

        u32 tmp = 0;
        while ((num = str_next_i32_0(&c, &(int){0}, &(int){0})) != -1)
            if (shashset_contains(&winning_nums, num))
                tmp += tmp == 0 ? 1 : tmp;

        sum += tmp;
        shashset_clear(&winning_nums);
    }

    fprintf(stdout, "Sum: %ld\n", sum);

    shashset_free(&winning_nums);
    free(line);
    fclose(fp);

    return 0;
}
