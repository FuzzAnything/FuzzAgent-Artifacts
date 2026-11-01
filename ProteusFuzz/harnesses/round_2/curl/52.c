// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void test_va_with_args(const char *format, ...) {
    va_list args;
    char buffer[1024];
    char *va_fmt_str = NULL;

    va_start(args, format);

    // Test curl_mvaprintf
    va_list args_copy;
    va_copy(args_copy, args);
    va_fmt_str = curl_mvaprintf(format, args_copy);
    va_end(args_copy);

    if (va_fmt_str) {
        curl_free(va_fmt_str);
    }

    // Test curl_mvsprintf
    va_list args_copy2;
    va_copy(args_copy2, args);
    int ret = curl_mvsprintf(buffer, format, args_copy2);
    va_end(args_copy2);
    (void)ret;

    va_end(args);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *easy_handle = NULL;
    CURL *dup_handle = NULL;
    char *env_var = NULL;
    char *fmt_str = NULL;

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a base handle for duplication
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Test curl_easy_duphandle
    dup_handle = curl_easy_duphandle(easy_handle);
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }

    // Test curl_version
    const char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    // Test curl_getenv
    if (Size >= 2 && Data[0] != 0) {
        size_t var_len = 0;
        while (var_len < Size && Data[var_len] != 0 && var_len < 256) {
            var_len++;
        }
        if (var_len > 0) {
            char *var_name = malloc(var_len + 1);
            if (var_name) {
                memcpy(var_name, Data, var_len);
                var_name[var_len] = '\0';
                env_var = curl_getenv(var_name);
                if (env_var) {
                    curl_free(env_var);
                }
                free(var_name);
            }
        }
    }

    // Test curl_maprintf
    size_t fmt_len = 0;
    while (fmt_len < Size && Data[fmt_len] != 0 && fmt_len < 128) {
        fmt_len++;
    }
    if (fmt_len > 0) {
        char *safe_fmt = malloc(fmt_len + 1);
        if (safe_fmt) {
            memcpy(safe_fmt, Data, fmt_len);
            safe_fmt[fmt_len] = '\0';
            
            // Use a fixed safe format to avoid undefined behavior with user input as format
            fmt_str = curl_maprintf("Input data: %s", safe_fmt);
            if (fmt_str) {
                curl_free(fmt_str);
            }
            free(safe_fmt);
        }
    }

    // Test curl_mvaprintf and curl_mvsprintf using a helper function that takes varargs
    const char *test_fmt = "Test: %s %d";
    const char *test_str = "fuzz";
    int test_int = 42;

    test_va_with_args(test_fmt, test_str, test_int);

    // Cleanup base handle
    curl_easy_cleanup(easy_handle);

    // Cleanup global environment
    curl_global_cleanup();

    return 0;
}