// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Initialize libcurl
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    // Create a dummy CURL handle for functions that require it
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Use the fuzz data as input string
    const char *input_string = (const char *)Data;
    int input_length = (int)Size;
    
    // Test curl_easy_escape
    if (Size > 0) {
        char *escaped = curl_easy_escape(handle, input_string, input_length);
        if (escaped) {
            curl_free(escaped);
        }
    }

    // Test curl_escape (deprecated)
    if (Size > 0) {
        char *escaped_dep = curl_escape(input_string, input_length);
        if (escaped_dep) {
            curl_free(escaped_dep);
        }
    }

    // Test curl_easy_unescape
    if (Size > 0) {
        int outlength = 0;
        char *unescaped = curl_easy_unescape(handle, input_string, input_length, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
    }

    // Test curl_unescape (deprecated)
    if (Size > 0) {
        char *unescaped_dep = curl_unescape(input_string, input_length);
        if (unescaped_dep) {
            curl_free(unescaped_dep);
        }
    }

    // Test curl_getenv - create a null-terminated environment variable name
    if (Size > 0 && Size < 256) {
        char env_name[256];
        memcpy(env_name, Data, Size);
        env_name[Size] = '\0';
        
        char *env_value = curl_getenv(env_name);
        if (env_value) {
            curl_free(env_value);
        }
    }

    // Test curl_free with various inputs
    if (Size > 0) {
        // Test with malloc'd memory
        void *test_mem = malloc(Size);
        if (test_mem) {
            memcpy(test_mem, Data, Size);
            curl_free(test_mem);
        }
        
        // Test with NULL
        curl_free(NULL);
    }

    // Clean up
    curl_easy_cleanup(handle);
    
    return 0;
}