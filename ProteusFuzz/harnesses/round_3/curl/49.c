// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
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

static void cleanup_fuzz_resources(char *escaped, char *unescaped, char *easy_unescaped, CURL *easy_handle, CURLM *multi_handle) {
    if (escaped) {
        curl_free(escaped);
    }
    if (unescaped) {
        curl_free(unescaped);
    }
    if (easy_unescaped) {
        curl_free(easy_unescaped);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    char *escaped = NULL;
    char *unescaped = NULL;
    char *easy_unescaped = NULL;
    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    int outlength = 0;
    int msgs_in_queue = 0;
    CURLMsg *msg = NULL;

    // Create handles
    easy_handle = curl_easy_init();
    multi_handle = curl_multi_init();

    // Use input data as source string for encoding/decoding
    // Ensure null termination for string functions
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test curl_escape
    escaped = curl_escape(input_str, Size);

    // 2. Test curl_unescape
    unescaped = curl_unescape(input_str, -1);

    // 3. Test curl_easy_unescape
    if (easy_handle) {
        easy_unescaped = curl_easy_unescape(easy_handle, input_str, Size, &outlength);
    }

    // 4. Test curl_msprintf
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // Fuzz format string with input data (careful with % specifiers)
    // We use a safe format string and pass input as argument
    int msprintf_result = curl_msprintf(buffer, "Input: %s", input_str);
    (void)msprintf_result; // Suppress unused warning

    // 5. Test curl_mprintf
    // Suppress output for fuzzing stability
    int mprintf_result = curl_mprintf("Fuzz: %s", input_str);
    (void)mprintf_result;

    // 6. Test curl_multi_info_read
    // This function needs a multi handle with messages. 
    // Since we haven't added any transfers, it should return NULL immediately.
    if (multi_handle) {
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        (void)msg;
        (void)msgs_in_queue;
    }

    // Cleanup
    cleanup_fuzz_resources(escaped, unescaped, easy_unescaped, easy_handle, multi_handle);
    free(input_str);
    curl_global_cleanup();

    return 0;
}