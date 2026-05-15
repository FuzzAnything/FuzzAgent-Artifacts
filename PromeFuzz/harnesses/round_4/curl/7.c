// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void test_curl_getenv(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated environment variable name from fuzzer input
    char *var_name = (char *)malloc(size + 1);
    if (!var_name) return;
    
    memcpy(var_name, data, size);
    var_name[size] = '\0';
    
    char *result = curl_getenv(var_name);
    if (result) {
        curl_free(result);
    }
    
    free(var_name);
}

static void test_curl_unescape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create string to unescape
    char *input = (char *)malloc(size + 1);
    if (!input) return;
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    // Test with actual length
    char *result1 = curl_unescape(input, (int)size);
    if (result1) {
        curl_free(result1);
    }
    
    // Test with null-terminated (length = 0)
    char *result2 = curl_unescape(input, 0);
    if (result2) {
        curl_free(result2);
    }
    
    free(input);
}

static void test_curl_easy_unescape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    CURL *curl = curl_easy_init();
    if (!curl) return;
    
    // Create string to unescape
    char *input = (char *)malloc(size + 1);
    if (!input) {
        curl_easy_cleanup(curl);
        return;
    }
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    int outlength = 0;
    
    // Test with actual length
    char *result1 = curl_easy_unescape(curl, input, (int)size, &outlength);
    if (result1) {
        curl_free(result1);
    }
    
    // Test with null-terminated (length = 0)
    char *result2 = curl_easy_unescape(curl, input, 0, &outlength);
    if (result2) {
        curl_free(result2);
    }
    
    free(input);
    curl_easy_cleanup(curl);
}

static void test_curl_easy_escape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    CURL *curl = curl_easy_init();
    if (!curl) return;
    
    // Create string to escape
    char *input = (char *)malloc(size + 1);
    if (!input) {
        curl_easy_cleanup(curl);
        return;
    }
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    // Test with actual length
    char *result1 = curl_easy_escape(curl, input, (int)size);
    if (result1) {
        curl_free(result1);
    }
    
    // Test with null-terminated (length = 0)
    char *result2 = curl_easy_escape(curl, input, 0);
    if (result2) {
        curl_free(result2);
    }
    
    free(input);
    curl_easy_cleanup(curl);
}

static void test_curl_escape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create string to escape
    char *input = (char *)malloc(size + 1);
    if (!input) return;
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    // Test with actual length
    char *result1 = curl_escape(input, (int)size);
    if (result1) {
        curl_free(result1);
    }
    
    // Test with null-terminated (length = 0)
    char *result2 = curl_escape(input, 0);
    if (result2) {
        curl_free(result2);
    }
    
    free(input);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Test all target functions with the fuzzer input
    
    test_curl_getenv(Data, Size);
    test_curl_unescape(Data, Size);
    test_curl_easy_unescape(Data, Size);
    test_curl_easy_escape(Data, Size);
    test_curl_escape(Data, Size);
    
    return 0;
}