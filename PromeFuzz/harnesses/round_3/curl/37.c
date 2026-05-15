// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void fuzz_curl_unescape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated string from fuzz data
    char *input = (char *)malloc(size + 1);
    if (!input) return;
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    // Call the target function
    char *result = curl_unescape(input, (int)size);
    
    // Free allocated resources
    if (result) {
        curl_free(result);
    }
    free(input);
}

static void fuzz_curl_strequal(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Split fuzz data into two strings
    size_t half = size / 2;
    char *str1 = (char *)malloc(half + 1);
    char *str2 = (char *)malloc(size - half + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return;
    }
    
    memcpy(str1, data, half);
    str1[half] = '\0';
    
    memcpy(str2, data + half, size - half);
    str2[size - half] = '\0';
    
    // Call the target function
    int result = curl_strequal(str1, str2);
    (void)result; // Use result to avoid unused variable warning
    
    free(str1);
    free(str2);
}

static void fuzz_curl_strnequal(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    // Split fuzz data into two strings and get n
    size_t half = size / 2;
    size_t n = data[0] % (half + 1); // Ensure n is within bounds
    
    char *str1 = (char *)malloc(half + 1);
    char *str2 = (char *)malloc(size - half + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return;
    }
    
    memcpy(str1, data, half);
    str1[half] = '\0';
    
    memcpy(str2, data + half, size - half);
    str2[size - half] = '\0';
    
    // Call the target function
    int result = curl_strnequal(str1, str2, n);
    (void)result; // Use result to avoid unused variable warning
    
    free(str1);
    free(str2);
}

static void fuzz_curl_easy_escape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Initialize CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) return;
    
    // Create null-terminated string from fuzz data
    char *input = (char *)malloc(size + 1);
    if (!input) {
        curl_easy_cleanup(curl);
        return;
    }
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    // Call the target function
    char *result = curl_easy_escape(curl, input, (int)size);
    
    // Free allocated resources
    if (result) {
        curl_free(result);
    }
    free(input);
    curl_easy_cleanup(curl);
}

static void fuzz_curl_easy_unescape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Initialize CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) return;
    
    // Create null-terminated string from fuzz data
    char *input = (char *)malloc(size + 1);
    if (!input) {
        curl_easy_cleanup(curl);
        return;
    }
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    int outlength = 0;
    
    // Call the target function with different length values
    for (int length_option = 0; length_option <= 1; length_option++) {
        int length = length_option == 0 ? 0 : (int)size;
        char *result = curl_easy_unescape(curl, input, length, &outlength);
        
        if (result) {
            curl_free(result);
        }
    }
    
    free(input);
    curl_easy_cleanup(curl);
}

static void fuzz_curl_escape(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated string from fuzz data
    char *input = (char *)malloc(size + 1);
    if (!input) return;
    
    memcpy(input, data, size);
    input[size] = '\0';
    
    // Call the target function with different length values
    for (int length_option = 0; length_option <= 1; length_option++) {
        int length = length_option == 0 ? 0 : (int)size;
        char *result = curl_escape(input, length);
        
        if (result) {
            curl_free(result);
        }
    }
    
    free(input);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *data, size_t size) {
    // Call all target functions with the fuzz input
    fuzz_curl_unescape(data, size);
    fuzz_curl_strequal(data, size);
    fuzz_curl_strnequal(data, size);
    fuzz_curl_easy_escape(data, size);
    fuzz_curl_easy_unescape(data, size);
    fuzz_curl_escape(data, size);
    
    return 0;
}