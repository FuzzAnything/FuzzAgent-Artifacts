// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

/*
 * Helper to extract a null-terminated string from fuzz input.
 * Returns a pointer to the string within Data or a static empty string.
 * Ensures we don't read past Size.
 */
static const char *get_string(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) {
        return "";
    }
    const char *start = (const char *)&Data[*offset];
    const char *end = start;
    size_t max_len = Size - *offset;
    size_t len = 0;
    
    while (len < max_len && end[len] != '\0') {
        len++;
    }
    *offset += len + 1; // Skip the null terminator if found, or remaining bytes
    if (len == max_len) {
        // No null terminator found within bounds, return empty string
        return "";
    }
    return start;
}

/*
 * Helper to get an integer from fuzz input.
 */
static int get_int(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) {
        return 0;
    }
    int val;
    memcpy(&val, &Data[*offset], 4);
    *offset += 4;
    return val;
}

/*
 * Helper function to handle va_list creation for fixed-argument functions
 * Using a variadic wrapper to satisfy va_start requirements
 */
static void fuzz_va_list_test(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    va_end(args);
    if (result != NULL) {
        sqlite3_free(result);
    }
}

/*
 * Helper to test sqlite3_str_vappendf with a variadic wrapper
 */
static void fuzz_str_va_list_test(sqlite3_str *s, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(s, fmt, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize SQLite if needed */
    sqlite3_initialize();

    /* Use a file for potential database context if needed */
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    size_t offset = 0;
    uint8_t mode = 0;
    if (Size > 0) {
        mode = Data[0];
        offset = 1;
    }

    switch (mode % 6) {
        case 0: {
            /* Fuzz sqlite3_mprintf */
            const char *fmt = get_string(Data, Size, &offset);
            int arg1 = get_int(Data, Size, &offset);
            int arg2 = get_int(Data, Size, &offset);
            char *result = sqlite3_mprintf(fmt, arg1, arg2);
            if (result != NULL) {
                sqlite3_free(result);
            }
            break;
        }
        case 1: {
            /* Fuzz sqlite3_snprintf */
            int n = get_int(Data, Size, &offset);
            if (n <= 0) n = 1;
            if (n > 1024) n = 1024; /* Limit buffer size for safety */
            char *buf = (char *)malloc(n);
            if (buf == NULL) break;
            
            const char *fmt = get_string(Data, Size, &offset);
            int arg1 = get_int(Data, Size, &offset);
            
            char *result = sqlite3_snprintf(n, buf, fmt, arg1);
            if (result != NULL) {
                /* result points to buf, so free buf */
            }
            free(buf);
            break;
        }
        case 2: {
            /* Fuzz sqlite3_vmprintf using helper to fix va_start issue */
            const char *fmt = get_string(Data, Size, &offset);
            int arg1 = get_int(Data, Size, &offset);
            int arg2 = get_int(Data, Size, &offset);
            
            fuzz_va_list_test(fmt, arg1, arg2);
            break;
        }
        case 3: {
            /* Fuzz sqlite3_str_new, sqlite3_str_appendf, sqlite3_str_finish */
            sqlite3_str *s = sqlite3_str_new(NULL);
            if (s == NULL) break;

            const char *fmt = get_string(Data, Size, &offset);
            int arg1 = get_int(Data, Size, &offset);
            int arg2 = get_int(Data, Size, &offset);
            
            sqlite3_str_appendf(s, fmt, arg1, arg2);
            
            /* Append some more data to test resizing */
            sqlite3_str_appendf(s, "%d %d", arg1, arg2);
            
            char *result = sqlite3_str_finish(s);
            if (result != NULL) {
                sqlite3_free(result);
            }
            break;
        }
        case 4: {
            /* Fuzz sqlite3_str_vappendf using helper */
            sqlite3_str *s = sqlite3_str_new(NULL);
            if (s == NULL) break;

            const char *fmt = get_string(Data, Size, &offset);
            int arg1 = get_int(Data, Size, &offset);
            int arg2 = get_int(Data, Size, &offset);
            
            fuzz_str_va_list_test(s, fmt, arg1, arg2);
            
            /* Append more */
            sqlite3_str_appendf(s, "extra");
            
            char *result = sqlite3_str_finish(s);
            if (result != NULL) {
                sqlite3_free(result);
            }
            break;
        }
        case 5: {
            /* Fuzz sqlite3_str with various append methods */
            sqlite3_str *s = sqlite3_str_new(NULL);
            if (s == NULL) break;

            const char *str = get_string(Data, Size, &offset);
            int n = get_int(Data, Size, &offset);
            if (n < 0) n = 0;
            if (n > 100) n = 100;
            
            /* Append fixed length */
            sqlite3_str_append(s, str, n);
            
            /* Append all */
            sqlite3_str_appendall(s, "suffix");
            
            /* Append char */
            sqlite3_str_appendchar(s, 5, 'x');
            
            /* Reset and truncate */
            sqlite3_str_reset(s);
            sqlite3_str_appendf(s, "reset_test");
            sqlite3_str_truncate(s, 2);
            
            char *result = sqlite3_str_finish(s);
            if (result != NULL) {
                sqlite3_free(result);
            }
            break;
        }
    }

    sqlite3_shutdown();
    return 0;
}