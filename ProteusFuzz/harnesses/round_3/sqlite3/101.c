// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sqlite3.h>

/* Helper to create a temporary null-terminated string from fuzz data */
static char *create_safe_string(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        char *s = sqlite3_malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = sqlite3_malloc(Size + 1);
    if (!s) return NULL;
    memcpy(s, Data, Size);
    s[Size] = '\0';
    return s;
}

/* Helper function to wrap va_start usage in a variadic context */
static void fuzz_vappendf(sqlite3_str *str, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(str, fmt, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    sqlite3_str *str = NULL;
    char *result = NULL;
    char *safe_str = NULL;

    /* 1. Prepare the environment */
    /* Initialize the dynamic string builder */
    str = sqlite3_str_new(NULL);
    if (!str) {
        return 0;
    }

    /* 2. Explore program states with diverse invocations */
    
    /* Case A: Append formatted string (sqlite3_str_appendf) */
    if (Size > 0) {
        safe_str = create_safe_string(Data, Size);
        if (safe_str) {
            sqlite3_str_appendf(str, safe_str);
            sqlite3_free(safe_str);
            safe_str = NULL;
        }
    }

    /* Case B: Append raw bytes (sqlite3_str_append) */
    if (Size > 0) {
        int n = (Size > 100) ? 100 : (int)Size;
        sqlite3_str_append(str, (const char *)Data, n);
    }

    /* Case C: Append full null-terminated string (sqlite3_str_appendall) */
    if (Size > 0) {
        safe_str = create_safe_string(Data, Size);
        if (safe_str) {
            sqlite3_str_appendall(str, safe_str);
            sqlite3_free(safe_str);
            safe_str = NULL;
        }
    }

    /* Case D: Variadic append (sqlite3_str_vappendf) */
    if (Size > 0) {
        safe_str = create_safe_string(Data, Size);
        if (safe_str) {
            fuzz_vappendf(str, safe_str);
            sqlite3_free(safe_str);
            safe_str = NULL;
        }
    }

    /* Case E: Reset and Truncate to explore state changes */
    sqlite3_str_reset(str);
    if (Size > 0) {
        int n = (Size > 50) ? 50 : (int)Size;
        sqlite3_str_truncate(str, n);
    }

    /* 3. Retrieve value and finalize */
    /* Check value before finishing */
    char *val = sqlite3_str_value(str);
    (void)val; 

    /* Finalize and get the result */
    result = sqlite3_str_finish(str);
    
    /* 4. Cleanup */
    if (result) {
        sqlite3_free(result);
    }

    return 0;
}