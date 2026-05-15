// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void *custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void *ptr) {
    free(ptr);
}

static void *custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *custom_strdup(const char *str) {
    return strdup(str);
}

static void *custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine which initialization path to take
    uint8_t init_selector = Data[0] % 3;
    
    // Initialize libcurl with different methods based on fuzzer input
    if (init_selector == 0) {
        // Try curl_global_sslset before curl_global_init
        curl_global_sslset((curl_sslbackend)(Data[0] % 10), NULL, NULL);
        curl_global_init(CURL_GLOBAL_ALL);
    } else if (init_selector == 1) {
        // Use curl_global_init_mem with custom memory functions
        curl_global_init_mem(CURL_GLOBAL_ALL,
                           custom_malloc,
                           custom_free,
                           custom_realloc,
                           custom_strdup,
                           custom_calloc);
    } else {
        // Standard curl_global_init
        curl_global_init(CURL_GLOBAL_ALL);
    }

    // Create a null-terminated string from fuzzer data for curl_global_trace
    char *trace_config = NULL;
    if (Size > 1) {
        size_t trace_len = Size - 1;
        if (trace_len > 1024) trace_len = 1024;
        trace_config = (char *)malloc(trace_len + 1);
        if (trace_config) {
            memcpy(trace_config, Data + 1, trace_len);
            trace_config[trace_len] = '\0';
            curl_global_trace(trace_config);
            free(trace_config);
        }
    }

    // Call curl_version_info with different version stamps
    curl_version_info_data *version_info = NULL;
    CURLversion versions[] = {CURLVERSION_FIRST, CURLVERSION_SECOND, CURLVERSION_THIRD,
                             CURLVERSION_FOURTH, CURLVERSION_FIFTH, CURLVERSION_SIXTH,
                             CURLVERSION_SEVENTH, CURLVERSION_EIGHTH, CURLVERSION_NINTH,
                             CURLVERSION_TENTH, CURLVERSION_ELEVENTH, CURLVERSION_TWELFTH,
                             CURLVERSION_NOW};
    
    size_t version_idx = Data[0] % (sizeof(versions) / sizeof(versions[0]));
    version_info = curl_version_info(versions[version_idx]);
    
    // Try curl_global_sslset again with different parameters after init
    if (Size > 2) {
        // Create a name string for sslset
        size_t name_len = Size - 2;
        if (name_len > 256) name_len = 256;
        char *ssl_name = (char *)malloc(name_len + 1);
        if (ssl_name) {
            memcpy(ssl_name, Data + 2, name_len);
            ssl_name[name_len] = '\0';
            curl_global_sslset(CURLSSLBACKEND_NONE, ssl_name, NULL);
            free(ssl_name);
        }
    }

    // Clean up
    curl_global_cleanup();

    return 0;
}