// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strequal at strequal.c:76:5 in curl.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>
#include <limits.h>

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file();

    CURL *handle = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();

    if (!handle || !multi_handle) {
        if (handle) curl_easy_cleanup(handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Fuzz curl_easy_unescape
    {
        int outlength = 0;
        const char *str = (const char *)Data;
        int len = (int)Size;
        if (len > INT_MAX) len = INT_MAX;
        if (len < 0) len = 0;

        char *decoded = curl_easy_unescape(handle, str, len, &outlength);
        if (decoded) {
            if (outlength > 0 && outlength < 1024) {
                char buf[1025];
                memset(buf, 0, sizeof(buf));
                strncpy(buf, decoded, 1024);
                (void)curl_strequal(buf, "test");
            }
            curl_free(decoded);
        }
    }

    // Fuzz curl_escape
    {
        const char *str = (const char *)Data;
        int len = (int)Size;
        if (len > INT_MAX) len = INT_MAX;
        if (len < 0) len = 0;

        char *encoded = curl_escape(str, len);
        if (encoded) {
            char buf[256];
            memset(buf, 0, sizeof(buf));
            int ret = curl_msprintf(buf, "encoded: %s", encoded);
            (void)ret;
            curl_free(encoded);
        }
    }

    // Fuzz curl_msprintf
    {
        char buffer[512];
        memset(buffer, 0, sizeof(buffer));
        // Create a safe, null-terminated format string from input
        char safe_format[512];
        size_t fmt_len = Size < sizeof(safe_format) - 1 ? Size : sizeof(safe_format) - 1;
        memcpy(safe_format, Data, fmt_len);
        safe_format[fmt_len] = '\0';

        int ret = curl_msprintf(buffer, safe_format, "test", 123, 45.6);
        (void)ret;
    }

    // Fuzz curl_mprintf
    {
        char safe_format[512];
        size_t fmt_len = Size < sizeof(safe_format) - 1 ? Size : sizeof(safe_format) - 1;
        memcpy(safe_format, Data, fmt_len);
        safe_format[fmt_len] = '\0';

        int ret = curl_mprintf(safe_format, "test", 123);
        (void)ret;
    }

    // Fuzz curl_strequal
    // Ensure both strings are null-terminated to prevent buffer overflows in the library
    {
        char s1_buf[256];
        char s2_buf[256];
        size_t copy_len = Size < sizeof(s1_buf) - 1 ? Size : sizeof(s1_buf) - 1;
        
        memcpy(s1_buf, Data, copy_len);
        s1_buf[copy_len] = '\0';
        
        // Create a second string by shifting or truncating to ensure diversity
        size_t s2_len = Size > 1 ? (Size - 1) : 0;
        if (s2_len >= sizeof(s2_buf)) s2_len = sizeof(s2_buf) - 1;
        memcpy(s2_buf, Data + 1, s2_len);
        s2_buf[s2_len] = '\0';

        int result = curl_strequal(s1_buf, s2_buf);
        (void)result;
    }

    // Fuzz curl_multi_info_read
    {
        int msgs_in_queue = 0;
        CURLMsg *msg = NULL;
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg) {
            (void)msg->msg;
            (void)msg->easy_handle;
            (void)msg->data.result;
        }
    }

    // Cleanup
    curl_easy_cleanup(handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}