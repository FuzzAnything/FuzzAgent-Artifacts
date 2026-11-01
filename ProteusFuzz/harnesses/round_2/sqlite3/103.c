// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3_str API functions.
 * Targets: sqlite3_str_appendchar, sqlite3_str_length, sqlite3_str_errcode,
 *          sqlite3_str_appendf, sqlite3_str_append, sqlite3_str_truncate.
 */

/* Helper to safely read integers from input */
static int get_int(const uint8_t *d, size_t *off, size_t size) {
    if (*off + 4 > size) return 0;
    int val = 0;
    memcpy(&val, d + *off, 4);
    *off += 4;
    return val;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    sqlite3_str *str = NULL;
    int n;
    char c;
    const char *fmt;
    const char *zIn;
    int nIn;
    int truncateN;

    if (Size == 0) {
        return 0;
    }

    /* Initialize a dynamic string object */
    str = sqlite3_str_new(NULL);
    if (str == NULL) {
        /* Cannot proceed without a valid string object */
        return 0;
    }

    size_t offset = 0;

    /* Fuzzing loop to explore various states */
    while (offset < Size) {
        /* Read an operation code */
        if (offset >= Size) break;
        int op = Data[offset++];

        switch (op & 0x0F) {
            case 0: /* sqlite3_str_appendchar */
                if (offset + 5 > Size) break;
                n = get_int(Data, &offset, Size);
                c = Data[offset++];
                /* Clamp n to reasonable bounds to avoid excessive allocation in fuzzing */
                if (n < 0) n = -n;
                if (n > 100000) n = 100000;
                sqlite3_str_appendchar(str, n, c);
                break;

            case 1: /* sqlite3_str_length */
                {
                    int len = sqlite3_str_length(str);
                    (void)len; /* Use the result to prevent unused warnings */
                }
                break;

            case 2: /* sqlite3_str_errcode */
                {
                    int err = sqlite3_str_errcode(str);
                    (void)err;
                }
                break;

            case 3: /* sqlite3_str_appendf */
                if (offset >= Size) break;
                /* Read format string length */
                int fmtLen = Data[offset++];
                if (offset + fmtLen > Size) break;
                
                /* Create a temporary buffer for the format string to ensure null termination */
                char *fmt_buf = (char *)malloc(fmtLen + 1);
                if (!fmt_buf) break;
                memcpy(fmt_buf, Data + offset, fmtLen);
                fmt_buf[fmtLen] = '\0';
                fmt = fmt_buf;
                offset += fmtLen;
                
                /* Read number of arguments (up to 4 for simplicity) */
                if (offset >= Size) break;
                int nargs = Data[offset++];
                if (nargs > 4) nargs = 4;
                
                /* We construct arguments based on input bytes */
                if (nargs == 0) {
                    sqlite3_str_appendf(str, fmt);
                } else if (nargs == 1) {
                    if (offset + 4 > Size) { free(fmt_buf); break; }
                    int arg1 = get_int(Data, &offset, Size);
                    sqlite3_str_appendf(str, fmt, arg1);
                } else if (nargs == 2) {
                    if (offset + 8 > Size) { free(fmt_buf); break; }
                    int arg1 = get_int(Data, &offset, Size);
                    int arg2 = get_int(Data, &offset, Size);
                    sqlite3_str_appendf(str, fmt, arg1, arg2);
                } else if (nargs == 3) {
                    if (offset + 12 > Size) { free(fmt_buf); break; }
                    int arg1 = get_int(Data, &offset, Size);
                    int arg2 = get_int(Data, &offset, Size);
                    int arg3 = get_int(Data, &offset, Size);
                    sqlite3_str_appendf(str, fmt, arg1, arg2, arg3);
                } else {
                    if (offset + 16 > Size) { free(fmt_buf); break; }
                    int arg1 = get_int(Data, &offset, Size);
                    int arg2 = get_int(Data, &offset, Size);
                    int arg3 = get_int(Data, &offset, Size);
                    int arg4 = get_int(Data, &offset, Size);
                    sqlite3_str_appendf(str, fmt, arg1, arg2, arg3, arg4);
                }
                free(fmt_buf);
                break;

            case 4: /* sqlite3_str_append */
                if (offset >= Size) break;
                nIn = Data[offset++];
                if (nIn < 0) nIn = -nIn;
                if (nIn > 10000) nIn = 10000;
                
                if (offset + nIn > Size) break;
                zIn = (const char *)(Data + offset);
                offset += nIn;
                
                sqlite3_str_append(str, zIn, nIn);
                break;

            case 5: /* sqlite3_str_truncate */
                if (offset + 4 > Size) break;
                truncateN = get_int(Data, &offset, Size);
                sqlite3_str_truncate(str, truncateN);
                break;

            default:
                /* No-op or random skip */
                break;
        }
    }

    /* Cleanup: Finish the string to free resources */
    char *result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }

    return 0;
}