#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../common.h"

int main(void)
{
    time_t t = clock();
    FILE *fp = fopen("./day4/input.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    SHASHSET(i32, i32);
    struct i32_shashset_t winning_nums;
    shashset_init(&winning_nums, HR_GLOBAL_ALLOCATOR, 13, cmp_i32, hash_int);

    u32 card_copies[209];
    for (u32 i = 0; i < 209; i++)
        card_copies[i] = 1;

    char *line = NULL;
    size_t len = 0;

    u64 sum = 0;
    u64 card_no = 0;
    while (getline(&line, &len, fp) != -1) {
        u8 *c = line;
        str_next_i32_0(&c, &(int){ 0 }, &(int){ 0 });

        i32 num = 0;
        while (*(c + 1) != '|' && (num = str_next_i32_0(&c, &(int){ 0 }, &(int){ 0 })) != -1)
            shashset_insert(&winning_nums, num);

        u32 wins = 0;
        while ((num = str_next_i32_0(&c, &(int){ 0 }, &(int){ 0 })) != -1)
            if (shashset_contains(&winning_nums, num))
                card_copies[card_no + 1 + wins++] += card_copies[card_no];

        shashset_clear(&winning_nums);
        card_no++;
    }

    fprintf(stdout, "Sum: %ld\n", ARRAY_BINOP_RESULT(card_copies, 209, +));

    shashset_free(&winning_nums);
    free(line);
    fclose(fp);
    t = clock() - t;

    printf("Execution time: %fms\n", ((double)t) / CLOCKS_PER_SEC * 1000);
    
    return 0;
}
