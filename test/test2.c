#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

int main(void)
{
	FILE *fp = fopen("./test/input.txt", "r");
	
    if (fp == NULL) {
        fprintf(stderr, "Error opening file. \n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    i64 tmp = 0;
    i64 val = 0;

    i64 maxes[3] = {0};

    while (getline(&line, &len, fp) != -1) {
        if ((val = atoi(line)) != 0){
            tmp += val;
        } else {
            if (tmp > maxes[0]) {
                maxes[0] = tmp;
                sort(maxes, 3, lambda(i64, (i64 a, i64 b), {
                    return a - b;
                }));
            }
            tmp = 0;
        }
    }

    if (tmp > maxes[0]) {
        maxes[0] = tmp;
        sort(maxes, 3, lambda(i64, (i64 a, i64 b), {
            return a - b;
        }));
    }

    u64 sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += maxes[i];
        printf("%lld\n", maxes[i]);
    }

    printf("Sum: %lld\n", sum);

    fclose(fp);

	return 0;
}
