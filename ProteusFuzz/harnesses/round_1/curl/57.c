// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Wrapper to allow va_start to work correctly by making the function variadic */
static int invoke_mvprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = curl_mvprintf(fmt, args);
    va_end(args);
    return r;
}

static int invoke_mvsprintf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = curl_mvsprintf(buf, fmt, args);
    va_end(args);
    return r;
}

static int invoke_mvfprintf(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = curl_mvfprintf(f, fmt, args);
    va_end(args);
    return r;
}

static void cleanup_allocated(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure we have enough data for basic operations */
    if (Size < 2) {
        return 0;
    }

    /* Create a null-terminated string from input for safe usage with %s */
    char *safe_input = malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    /* Prepare a buffer for curl_msnprintf and curl_mvsprintf */
    size_t buf_size = Size + 1024;
    char *buffer = malloc(buf_size);
    if (!buffer) {
        free(safe_input);
        return 0;
    }
    memset(buffer, 0, buf_size);

    /* Test 1: curl_msnprintf */
    int ret1 = curl_msnprintf(buffer, buf_size, "Input length: %zu, Data: %s", Size, safe_input);
    (void)ret1;

    /* Test 2: curl_maprintf */
    char *allocated_result = curl_maprintf("Fuzzed: %s (len=%zu)", safe_input, Size);
    if (allocated_result) {
        size_t len = strlen(allocated_result);
        (void)len;
        cleanup_allocated(allocated_result);
    }

    /* Test 3: curl_mvprintf via helper */
    int ret3 = invoke_mvprintf("Debug: %s (%zu)", safe_input, Size);
    (void)ret3;

    /* Test 4: curl_mvsprintf via helper */
    int ret4 = invoke_mvsprintf(buffer, "MVSprintf: %s (%zu)", safe_input, Size);
    (void)ret4;

    /* Test 5: curl_mvfprintf via helper */
    FILE *fd = fopen("./dummy_file", "w");
    if (fd) {
        int ret6 = invoke_mvfprintf(fd, "File: %s (%zu)\n", safe_input, Size);
        (void)ret6;
        fclose(fd);
    }

    /* Cleanup */
    free(buffer);
    free(safe_input);

    return 0;
}