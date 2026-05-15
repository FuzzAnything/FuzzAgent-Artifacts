// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:19130:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void vappendf_wrapper(sqlite3_str *p, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(p, fmt, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return 0;
    
    uint8_t op = Data[0] % 6;
    const uint8_t *input = Data + 1;
    size_t input_size = Size - 1;
    
    char *buf = NULL;
    if (input_size > 0) {
        buf = (char*)malloc(input_size + 1);
        if (!buf) {
            sqlite3_str_finish(str);
            return 0;
        }
        memcpy(buf, input, input_size);
        buf[input_size] = '\0';
    } else {
        buf = (char*)malloc(1);
        if (!buf) {
            sqlite3_str_finish(str);
            return 0;
        }
        buf[0] = '\0';
    }
    
    switch (op) {
        case 0:
            if (buf[0] != '\0') {
                sqlite3_str_appendall(str, buf);
            }
            break;
        case 1:
            if (input_size > 0) {
                sqlite3_str_append(str, buf, (int)(input_size % 1024));
            }
            break;
        case 2:
            sqlite3_str_appendf(str, "%s %d", buf, 123);
            break;
        case 3:
            vappendf_wrapper(str, "%s %d", buf, 456);
            break;
        case 4:
            sqlite3_str_value(str);
            break;
        case 5:
            char *result = sqlite3_str_finish(str);
            if (result) sqlite3_free(result);
            str = NULL;
            break;
    }
    
    free(buf);
    
    if (str) {
        char *result = sqlite3_str_finish(str);
        if (result) sqlite3_free(result);
    }
    
    return 0;
}