// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/easy.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
        // Fuzz curl_multi_get_handles
        CURL **handles = curl_multi_get_handles(multi_handle);
        if (handles) {
            // Just iterate to ensure no crash, we don't need to do anything with handles
            for (int i = 0; handles[i] != NULL; i++) {
                (void)handles[i];
            }
            curl_free(handles);
        }
    }

    // Fuzz curl_maprintf with various formats from input
    // Ensure we have enough data for format and at least one argument
    if (Size >= 2) {
        char format[16];
        size_t fmt_len = (Size < 16) ? Size : 16;
        memcpy(format, Data, fmt_len);
        format[fmt_len] = '\0';

        // Ensure format string is valid and safe
        // Replace any '%' with 'x' to avoid unexpected format specifiers
        for (size_t i = 0; i < fmt_len; i++) {
            if (format[i] == '%') {
                format[i] = 'x';
            }
        }

        // Use data from input as arguments, ensuring bounds
        int arg1 = Data[fmt_len % Size];
        int arg2 = Data[(fmt_len + 1) % Size];

        char *result = curl_maprintf(format, arg1, arg2);
        if (result) {
            curl_free(result);
        }
    }

    // Fuzz curl_maprintf with more arguments
    if (Size >= 3) {
        char format[16];
        size_t fmt_len = (Size < 16) ? Size : 16;
        memcpy(format, Data, fmt_len);
        format[fmt_len] = '\0';

        // Ensure format string is valid and safe
        for (size_t i = 0; i < fmt_len; i++) {
            if (format[i] == '%') {
                format[i] = 'x';
            }
        }

        int arg1 = Data[fmt_len % Size];
        int arg2 = Data[(fmt_len + 1) % Size];
        int arg3 = Data[(fmt_len + 2) % Size];

        char *result = curl_maprintf(format, arg1, arg2, arg3);
        if (result) {
            curl_free(result);
        }
    }

    // Fuzz curl_version
    char *version = curl_version();
    if (version) {
        (void)strlen(version); // Just use it to avoid unused warning
    }

    // Fuzz curl_easy_reset
    curl_easy_reset(easy_handle);

    // Cleanup multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);

    // Cleanup easy handle
    curl_easy_cleanup(easy_handle);

    // Cleanup global environment
    curl_global_cleanup();

    return 0;
}