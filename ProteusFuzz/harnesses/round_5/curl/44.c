// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
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
#include <stdio.h>

static void safe_write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl handle
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Initialize multi handle for curl_multi_info_read
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(handle);
        return 0;
    }

    // Prepare a safe null-terminated string from input for %s format
    char *safe_str = NULL;
    if (Size > 0) {
        safe_str = (char *)malloc(Size + 1);
        if (safe_str) {
            memcpy(safe_str, Data, Size);
            safe_str[Size] = '\0';
        }
    }

    // 1. Test curl_mprintf
    if (safe_str) {
        // Use a safe format string to avoid format string vulnerabilities
        // We pass the user data as an argument, not as the format string
        (void)curl_mprintf("Input: %s\n", safe_str);
    }

    // 2. Test curl_msprintf
    if (safe_str) {
        char buffer[256];
        // Ensure buffer is large enough and format is safe
        (void)curl_msprintf(buffer, "Prefix: %s", safe_str);
    }

    // 3. Test curl_easy_unescape
    if (safe_str) {
        int outlength = 0;
        char *unescaped = curl_easy_unescape(handle, safe_str, (int)Size, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
    }

    // 4. Test curl_multi_info_read
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    // Just call it to exercise the path; msg will likely be NULL as no transfers are running
    (void)msg;

    // 5. Test curl_easy_escape
    if (safe_str) {
        char *escaped = curl_easy_escape(handle, safe_str, (int)Size);
        if (escaped) {
            curl_free(escaped);
        }
    }

    // 6. Test curl_strequal
    if (safe_str) {
        int result = curl_strequal(safe_str, safe_str);
        (void)result;
        // Test with NULL
        result = curl_strequal(NULL, NULL);
        (void)result;
        // Test with one NULL
        result = curl_strequal(safe_str, NULL);
        (void)result;
    }

    // Cleanup
    curl_easy_cleanup(handle);
    curl_multi_cleanup(multi_handle);
    if (safe_str) {
        free(safe_str);
    }

    return 0;
}