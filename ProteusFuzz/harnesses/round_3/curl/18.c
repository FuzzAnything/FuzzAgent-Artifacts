// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback to satisfy type checking requirements if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

/* Variadic helper to safely call curl_mvsnprintf */
static void fuzz_mvsnprintf(const char *format, char *buffer, size_t maxlength, ...) {
    va_list args;
    va_start(args, maxlength);
    curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Prepare safe format string */
    char safe_format[256];
    size_t fmt_len = (Size > 10) ? (Size - 10) : Size;
    if (fmt_len >= sizeof(safe_format)) fmt_len = sizeof(safe_format) - 1;
    const char *format_src = (Size > 10) ? (const char *)(Data + 10) : (const char *)Data;
    memcpy(safe_format, format_src, fmt_len);
    safe_format[fmt_len] = '\0';

    /* 1. Test curl_msnprintf */
    char buffer_msnprintf[4096];
    int ret_msnprintf = curl_msnprintf(buffer_msnprintf, sizeof(buffer_msnprintf), safe_format, Data[0], Data[1]);
    (void)ret_msnprintf;

    /* 2. Test curl_mvsnprintf via the variadic helper */
    char buffer_mvsnprintf[4096];
    fuzz_mvsnprintf(safe_format, buffer_mvsnprintf, sizeof(buffer_mvsnprintf), Data[0], Data[1]);

    /* 3. Test curl_strnequal */
    const char *s1 = (Size > 5) ? (const char *)(Data + 5) : (const char *)Data;
    const char *s2 = (Size > 10) ? (const char *)(Data + 10) : (const char *)Data;
    size_t n = (Size > 2) ? Data[2] : 10;
    int ret_strnequal = curl_strnequal(s1, s2, n);
    (void)ret_strnequal;

    /* 4. Test curl_multi_get_handles */
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            /* Add handle to multi */
            if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
                CURL **handles = curl_multi_get_handles(multi_handle);
                if (handles) {
                    /* Iterate and free */
                    for (int i = 0; handles[i] != NULL; i++) {
                        /* Inspect handle if needed */
                        (void)handles[i];
                    }
                    curl_free(handles);
                }
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            curl_easy_cleanup(easy_handle);
        }
        curl_multi_cleanup(multi_handle);
    }

    /* Cleanup curl global */
    curl_global_cleanup();

    return 0;
}