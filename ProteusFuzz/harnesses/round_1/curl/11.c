// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
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
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for testing");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file();

    CURL *handle = NULL;
    CURLM *multi_handle = NULL;
    char *escaped = NULL;
    char *unescaped = NULL;
    int outlength = 0;
    int msgs_in_queue = 0;
    CURLMsg *msg = NULL;
    char buffer[256];

    // Initialize curl global
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    // Create easy handle
    handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, handle);

    // Fuzz curl_escape
    // Ensure we have at least 1 byte for string
    if (Size > 0) {
        // Limit length to avoid huge allocations
        int len = (Size < 1024) ? (int)Size : 1024;
        // Ensure null termination for safety in case string is not null-terminated
        char *safe_string = (char *)malloc(len + 1);
        if (safe_string) {
            memcpy(safe_string, Data, len);
            safe_string[len] = '\0';
            escaped = curl_escape(safe_string, len);
            if (escaped) {
                curl_free(escaped);
            }
            free(safe_string);
        }
    }

    // Fuzz curl_easy_unescape
    if (Size > 0) {
        int len = (Size < 1024) ? (int)Size : 1024;
        unescaped = curl_easy_unescape(handle, (const char *)Data, len, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
    }

    // Fuzz curl_strequal
    if (Size >= 2) {
        char s1[64] = {0};
        char s2[64] = {0};
        int len1 = (Size / 2) < 63 ? (int)(Size / 2) : 63;
        int len2 = (Size - len1) < 63 ? (int)(Size - len1) : 63;
        memcpy(s1, Data, len1);
        memcpy(s2, Data + len1, len2);
        curl_strequal(s1, s2);
        curl_strequal(NULL, NULL);
        curl_strequal(s1, NULL);
        curl_strequal(NULL, s2);
    }

    // Fuzz curl_mprintf
    if (Size > 0) {
        char format_str[64] = {0};
        int fmt_len = (Size < 63) ? (int)Size : 63;
        memcpy(format_str, Data, fmt_len);
        // Ensure format string is safe by limiting to simple cases
        // We just call it to trigger internal logic, ignoring potential format errors
        curl_mprintf("%s", format_str);
        curl_mprintf("%d", (int)Data[0]);
    }

    // Fuzz curl_msprintf
    if (Size > 0) {
        char format_str[64] = {0};
        int fmt_len = (Size < 63) ? (int)Size : 63;
        memcpy(format_str, Data, fmt_len);
        curl_msprintf(buffer, "%s", format_str);
        curl_msprintf(buffer, "%d", (int)Data[0]);
    }

    // Fuzz curl_multi_info_read
    // This function reads from the queue. We call it multiple times.
    msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    while (msg != NULL) {
        // Just consume the message, no specific action needed for fuzzing
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}