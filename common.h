#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hurust.h"
#define HURUST_IMPLEMENTATION

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

int f_next_item(FILE *restrict fp, u8 *restrict buf, u32 buf_len)
{
    u8 ch;
    u32 i = 0;

    while ((ch = fgetc(fp)) == ' ')
        ;

    if (ch == EOF)
        return -1;

    buf[i++] = ch;

    do {
        ch = fgetc(fp);
        if (ch == ' ' || ch == '\t') {
            buf[i] = '\0';
            break;
        } else if (ch == '\n') {
            break;
        } else if (i >= buf_len) {
            fprintf(stderr, "Error parsing file: item longer than %d chars\n", buf_len);
            return -1;
        }

        buf[i++] = ch;
    } while (ch != EOF);

    if (ch == EOF)
        return -1;

    return 1;
}

int f_next_str(FILE *restrict fp, char *restrict buf, u32 buf_len)
{
    u8 ch;
    u64 i = 0;

    while ((ch = fgetc(fp)) == ' ')
        ;

    if (ch == EOF)
        return -1;

    ch = fgetc(fp);

    buf[i++] = ch;

    do {
        ch = fgetc(fp);
        if (ch == '"') {
            buf[i] = '\0';
            break;
        } else if (ch == '\n') {
            break;
        } else if (i >= buf_len) {
            fprintf(stderr, "Error parsing file: item longer than %d chars\n", buf_len);
            return -1;
        }

        buf[i++] = ch;
    } while (ch != EOF);

    if (ch == EOF)
        return -1;

    return 1;
}

i32 f_next_int(FILE *restrict fp)
{
    u8 buf[11] = { 0 };

    i32 rc = f_next_item(fp, buf, 11);
    if (rc == -1) {
        fprintf(stderr, "Null from f_next_item.\n");
        return -1;
    }

    i32 ret = atoi(buf);
    return ret;
}

f64 f_next_double(FILE *restrict fp)
{
    u8 buf[11] = { 0 };

    i32 rc = f_next_item(fp, buf, 11);
    if (rc == -1) {
        fprintf(stderr, "Null from f_next_item.\n");
        return -1;
    }

    f64 ret = atof(buf);
    return ret;
}

void consume_line(FILE *restrict fp)
{
    u8 ch;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
        ;
}