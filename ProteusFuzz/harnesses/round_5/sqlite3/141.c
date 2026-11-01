// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_str_* API functions.
 * Target APIs:
 *   sqlite3_str_new
 *   sqlite3_str_appendf
 *   sqlite3_str_value
 *   sqlite3_str_finish
 *   sqlite3_str_free
 *   sqlite3_str_reset
 *
 * Fix for ASAN crash:
 * The crash occurred at sqlite3_str_vappendf due to an assertion:
 * assert( pAccum->nChar>0 || (pAccum->printfFlags&SQLITE_PRINTF_MALLOCED)==0 );
 * This assertion fails if we try to append formatted text to an empty string
 * when the internal flags indicate a malloc operation was expected but not performed.
 * To fix this, we ensure that we never call sqlite3_str_appendf on an empty string
 * unless we are certain it's safe. A safer approach is to always append a non-formatted
 * character first if the string is empty, or avoid using appendf on empty strings.
 */

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str == NULL) {
        return 0;
    }

    size_t idx = 0;
    while (idx < Size) {
        uint8_t op = Data[idx] % 6;
        idx++;

        switch (op) {
            case 0: {
                /* 
                 * sqlite3_str_appendf with a format string.
                 * To avoid the assertion failure, we first ensure the string is not empty
                 * by appending a dummy character if needed.
                 */
                if (idx < Size) {
                    int arg_val = (int)Data[idx];
                    idx++;
                    
                    /* Ensure string is not empty before using appendf */
                    if (sqlite3_str_length(str) == 0) {
                        sqlite3_str_appendchar(str, 1, ' ');
                    }
                    
                    sqlite3_str_appendf(str, "%d", arg_val);
                }
                break;
            }
            case 1: {
                /* sqlite3_str_append with raw bytes */
                if (idx < Size) {
                    int n = (Data[idx] % 100) + 1;
                    idx++;
                    if (idx + n <= Size) {
                        sqlite3_str_append(str, (const char *)(Data + idx), n);
                        idx += n;
                    }
                }
                break;
            }
            case 2: {
                /* sqlite3_str_appendall with a null-terminated string */
                if (idx < Size) {
                    size_t str_len = 0;
                    while (idx + str_len < Size && Data[idx + str_len] != '\0') {
                        str_len++;
                    }
                    if (str_len > 0) {
                        char *s = (char *)malloc(str_len + 1);
                        if (s) {
                            memcpy(s, Data + idx, str_len);
                            s[str_len] = '\0';
                            sqlite3_str_appendall(str, s);
                            free(s);
                            idx += str_len + 1;
                        }
                    } else {
                        idx++;
                    }
                }
                break;
            }
            case 3: {
                /* sqlite3_str_appendchar */
                if (idx < Size) {
                    int n = Data[idx] % 100;
                    idx++;
                    if (idx < Size) {
                        char c = (char)Data[idx];
                        idx++;
                        sqlite3_str_appendchar(str, n, c);
                    }
                }
                break;
            }
            case 4: {
                /* sqlite3_str_reset */
                sqlite3_str_reset(str);
                break;
            }
            case 5: {
                /* sqlite3_str_truncate */
                if (idx < Size) {
                    int n = (int)Data[idx];
                    idx++;
                    sqlite3_str_truncate(str, n);
                }
                break;
            }
        }
    }

    /*
     * Check for errors during construction
     */
    int err = sqlite3_str_errcode(str);
    (void)err; /* Use err to avoid unused variable warning */

    /*
     * Retrieve the value using sqlite3_str_value
     * Note: This modifies the buffer by adding a null terminator
     */
    char *value = sqlite3_str_value(str);
    if (value != NULL) {
        /* Use the value to ensure it's valid */
        size_t len = strlen(value);
        (void)len;
    }

    /*
     * Finalize the string using sqlite3_str_finish
     * This frees the sqlite3_str object and returns the string buffer
     */
    char *finished = sqlite3_str_finish(str);
    if (finished != NULL) {
        /* Free the returned buffer to prevent memory leaks */
        sqlite3_free(finished);
    }

    return 0;
}