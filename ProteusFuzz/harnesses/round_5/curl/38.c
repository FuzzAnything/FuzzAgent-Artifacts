// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Helper to safely create a null-terminated string from fuzz input */
static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

/* Wrapper to initialize va_list for curl_mvfprintf and curl_mvprintf */
static void test_mvfprintf(FILE *fp, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fp, format, args);
    va_end(args);
}

static void test_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

/* Wrapper to initialize va_list for curl_mvsprintf */
static void test_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare a safe string from input for format specifiers that expect strings */
    char *safe_input = create_safe_string(Data, Size);
    if (!safe_input) {
        return 0;
    }

    /* 1. Test curl_mvfprintf */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        /* Case 1: %s format */
        test_mvfprintf(fp, "%s", safe_input);

        /* Case 2: %d format using input byte as int */
        test_mvfprintf(fp, "%d", (int)Data[0]);

        /* Case 3: %x format */
        test_mvfprintf(fp, "%x", (unsigned int)Data[0]);

        fclose(fp);
    }

    /* 2. Test curl_mvprintf */
    test_mvprintf("%s", safe_input);
    test_mvprintf("%d", (int)Data[0]);

    /* 3. Test curl_maprintf */
    /* Ensure string argument is valid and null-terminated (already done in safe_input) */
    char *result1 = curl_maprintf("%s", safe_input);
    if (result1) {
        curl_free(result1);
    }

    char *result2 = curl_maprintf("%d %s", (int)Data[0], safe_input);
    if (result2) {
        curl_free(result2);
    }

    /* 4. Test curl_msnprintf */
    char buffer[256];
    int ret1 = curl_msnprintf(buffer, sizeof(buffer), "%s", safe_input);
    (void)ret1;

    int ret2 = curl_msnprintf(buffer, sizeof(buffer), "%d %s", (int)Data[0], safe_input);
    (void)ret2;

    /* 5. Test curl_mvsprintf */
    char buffer2[256];
    test_mvsprintf(buffer2, "%s", safe_input);
    test_mvsprintf(buffer2, "%d %s", (int)Data[0], safe_input);

    /* 6. The Wcurl_* macros are compile-time type checkers and do not have runtime logic
       to fuzz directly. They are invoked implicitly by curl_easy_setopt etc.
       Since we cannot easily trigger the macro expansion logic without a full curl_easy_setopt
       call which requires a CURL handle and complex setup, we skip direct invocation of these
       macros as they are not functions. */

    /* Cleanup */
    free(safe_input);

    return 0;
}