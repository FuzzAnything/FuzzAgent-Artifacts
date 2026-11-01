// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>
#include <stdarg.h>

/*
 * Fuzz driver for curl library functions:
 * - curl_easy_escape
 * - curl_mfprintf
 * - curl_mvfprintf
 *
 * Note: The Wcurl_* macros are compile-time type checkers and do not
 * require runtime invocation. They are included via headers to ensure
 * the build environment is consistent, but they do not generate runtime
 * code paths to fuzz directly.
 */

static void write_dummy_file(const char *path, const uint8_t *data, size_t size) {
    FILE *f = fopen(path, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

/* Helper function to test curl_mvfprintf */
static int test_mvfprintf(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = curl_mvfprintf(f, fmt, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * 1. Fuzz curl_easy_escape
     * It takes a handle, a string, and a length.
     * We use the fuzz input as the string.
     */
    char *escaped = curl_easy_escape(handle, (const char *)Data, (int)Size);
    if (escaped) {
        /* Perform some operation to ensure the memory is touched */
        size_t len = strlen(escaped);
        if (len > 0) {
            volatile char c = escaped[len - 1];
            (void)c;
        }
        curl_free(escaped);
    }

    /*
     * 2. Fuzz curl_mfprintf
     * We create a dummy file and write the fuzz data as a format string
     * or arguments. To be safe, we avoid using the raw fuzz data as a
     * format string directly (which could cause crashes in some sanitizers).
     * Instead, we write the data as a simple string.
     */
    const char *dummy_path = "./dummy_file";
    write_dummy_file(dummy_path, Data, Size);

    FILE *fp = fopen(dummy_path, "w");
    if (fp) {
        /* Write a simple format to test the function */
        int ret = curl_mfprintf(fp, "Data size: %zu\n", Size);
        (void)ret;

        /* Write some bytes from input as a string (assuming printable or safe) */
        /* We limit to avoid huge writes */
        size_t write_len = (Size < 1024) ? Size : 1024;
        for (size_t i = 0; i < write_len; i++) {
            unsigned char c = Data[i];
            /* Only write printable characters to avoid control char issues */
            if (c >= 32 && c < 127) {
                curl_mfprintf(fp, "%c", c);
            }
        }
        fclose(fp);
    }

    /*
     * 3. Fuzz curl_mvfprintf
     * Similar to curl_mfprintf, but using va_list.
     */
    fp = fopen(dummy_path, "a");
    if (fp) {
        int ret2 = test_mvfprintf(fp, "Size: %zu, First byte: %u\n", Size, (Size > 0) ? Data[0] : 0);
        (void)ret2;
        fclose(fp);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}