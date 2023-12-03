#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HURUST_IMPLEMENTATION
#include "hurust.h"

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

// String parsing functions
int str_next_item(u8 *restrict inn, u8 *restrict buf, u32 buf_len)
{
    u32 i = 0;

    u8 *c = inn;
    while (*c == ' ')
        c++;

    if (*c == '\0')
        return -1;

    buf[i++] = *c;

    do {
        c++;
        if (*c == ' ' || *c == '\t') {
            buf[i] = '\0';
            break;
        } else if (*c == '\n') {
            break;
        } else if (i >= buf_len) {
            return -1;
        }

        buf[i++] = *c;
    } while (*c != '\0');

    if (*c == '\0')
        return -1;

    return 1;
}

int __str_next_str(u8 *restrict inn, char *restrict buf, u32 buf_len)
{
    u64 i = 0;
    u8 *c = inn;

    while (*c == ' ')
        c++;

    if (*c == '\0')
        return -1;

    buf[i++] = *c;

    do {
        c++;
        if (*c == '"') {
            buf[i] = '\0';
            break;
        } else if (*c == '\n') {
            break;
        } else if (i >= buf_len) {
            return -1;
        }

        buf[i++] = *c;
    } while (*c != '\0');

    if (*c == '\0')
        return -1;

    return 1;
}

char *str_next_str(u8 **restrict data)
{
    char buf[256] = { 0 };
    u8 *c = *data;

    i32 rc = __str_next_str(c, buf, 256);
    if (rc == -1) {
        fprintf(stderr, "str_next_str: __str_next_str returned -1.\n");
        return NULL;
    }

    *data += strlen(buf) + 1;

    char *ret = malloc(strlen(buf) + 1);
    strcpy(ret, buf);
    return ret;
}

i32 str_next_i32_0(u8 **restrict data, i32 *start, i32 *end)
{
    u8 buf[11] = { 0 };
    u8 *c = *data;

    u32 ws_count = 0;
    while (!isdigit(*c)) {
        if (*c == '\0')
            return -1;
        c++;
        ws_count++;
    }

    *start = ws_count;
    u32 i = 0;
    do {
        if (!isdigit(*c)) {
            buf[i] = '\0';
            break;
        } else if (*c == '\n') {
            break;
        } else if (i >= 11) {
            return -1;
        }

        buf[i++] = *c++;
    } while (*c != '\0');

    if (i == 0)
        return -1;

    *end = i + ws_count - 1;
    *data += *end + 1;

    return atoi(buf);
}

i32 str_next_i32(u8 **restrict data)
{
    u8 buf[11] = { 0 };
    u8 *c = *data;

    u32 ws_count = 0;
    while (!isdigit(*c)) {
        if (*c == '\0')
            return -1;
        c++;
        ws_count++;
    }

    i32 rc = str_next_item(c, buf, 11);
    if (rc == -1) {
        fprintf(stderr, "str_next_i32: str_next_item returned -1.\n");
        return -1;
    }

    *data += strlen(buf) + ws_count - 1;

    i32 ret = atoi(buf);
    return ret;
}

f64 str_next_f64(u8 **restrict data)
{
    u8 buf[128] = {
        0
    }; // Could be 1074, but when would you ever need to parse a number with 1074 digits?
    u8 *c = *data;

    u32 ws_count = 0;
    while (!isdigit(*c)) {
        if (*c == '\0')
            return -1;
        c++;
        ws_count++;
    }

    i32 rc = str_next_item(c, buf, 11);
    if (rc == -1) {
        fprintf(stderr, "str_next_f64: str_next_item returned -1.\n");
        return -1;
    }

    *data += strlen(buf) + ws_count - 1;

    f64 ret = atof(buf);
    return ret;
}

/* Cursed macros that are added for funsies and because I can. */
#define ARRAY_BINOP_RESULT(a, len, op) \
    ({                                 \
        i64 ret = a[0];                \
        for (u32 i = 1; i < len; i++)  \
            ret = ret op a[i];         \
        ret;                           \
    })

#define SET_LARGEST_PRIMITIVE(a, b) (a = a > b ? a : b)
#define SET_LARGEST(a, b, cmp) (a = cmp(a, b) ? a : b)

#define SET_SMALLEST_PRIMITIVE(a, b) (a = a < b ? a : b)
#define SET_SMALLEST(a, b, cmp) (a = cmp(a, b) ? a : b)

// This next section is taken from my JsonStruct project
// Eval obfuscation loop to allow for recursive macro expansion by keeping tokens unevaluated
// Each level multiplies the effort of the level before, evaluating the input 365 times in total.
// In other words, calling EVAL (A (blah)) would produce 365 copies of the word blah, followed by a
// final un-evaluated B (blah). This provides the basic framework for recursion, at least within a
// certain stack depth.
#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL5(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL(...) EVAL4(__VA_ARGS__)

#define NOP

// Recursive macro's cannot call themselves directly, so we need helper macros to do the recursion
// for us.
#define MAP_POP_TWO_ARGS0(F, X, Y, ...) F(X, Y) __VA_OPT__(MAP_POP_TWO_ARGS1 NOP(F, __VA_ARGS__))
#define MAP_POP_TWO_ARGS1(F, X, Y, ...) F(X, Y) __VA_OPT__(MAP_POP_TWO_ARGS0 NOP(F, __VA_ARGS__))

#define MAP_TWO_ARGS(F, ...) __VA_OPT__(EVAL(MAP_POP_TWO_ARGS0(F, __VA_ARGS__)))

// Macro for generating a sim
#define MAP_ENTRY(key, val) { key, val },

#define GEN_MAP(name, keytype, valtype, eq_fn, ...)            \
    static struct name##_map_entry_t {                         \
        keytype key;                                           \
        valtype val;                                           \
    } name##_map[] = { MAP_TWO_ARGS(MAP_ENTRY, __VA_ARGS__) }; \
    valtype name##_map_get(const keytype key)                  \
    {                                                          \
        struct name##_map_entry_t *ptr = name##_map;           \
        for (; ptr->key && !eq_fn(key, ptr->key); ++ptr)       \
            ;                                                  \
        return ptr->val;                                       \
    }
