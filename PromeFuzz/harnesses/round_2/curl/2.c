// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdlib.h>
#include <string.h>

static void *dummy_malloc(size_t size) {
    return malloc(size);
}

static void dummy_free(void *ptr) {
    free(ptr);
}

static void *dummy_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *dummy_strdup(const char *str) {
    return strdup(str);
}

static void *dummy_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to decide which initialization path to take
    uint8_t init_type = Data[0] % 3;
    CURLcode init_result;
    
    switch (init_type) {
        case 0:
            // Use curl_global_init with flags derived from input
            long flags = 0;
            if (Size > 1) {
                flags = (long)Data[1];
            }
            init_result = curl_global_init(flags);
            break;
            
        case 1:
            // Use curl_global_init_mem with dummy callbacks
            long flags_mem = 0;
            if (Size > 1) {
                flags_mem = (long)Data[1];
            }
            init_result = curl_global_init_mem(flags_mem,
                                              dummy_malloc,
                                              dummy_free,
                                              dummy_realloc,
                                              dummy_strdup,
                                              dummy_calloc);
            break;
            
        default:
            // Don't initialize at all for some paths
            init_result = CURLE_OK;
            break;
    }

    // Always call version functions regardless of initialization
    char *version_str = curl_version();
    (void)version_str; // Use to avoid unused variable warning
    
    // Call version_info with different version requests
    CURLversion version_request = CURLVERSION_NOW;
    if (Size > 2) {
        version_request = (CURLversion)(Data[2] % 12); // CURLVERSION_LAST is 12
    }
    curl_version_info_data *version_info = curl_version_info(version_request);
    (void)version_info; // Use to avoid unused variable warning
    
    // Call curl_global_trace with config string from input
    if (Size > 3 && init_result == CURLE_OK) {
        // Create null-terminated string from input data
        size_t config_len = Size - 3;
        if (config_len > 256) {
            config_len = 256;
        }
        char *config = (char *)malloc(config_len + 1);
        if (config) {
            memcpy(config, Data + 3, config_len);
            config[config_len] = '\0';
            CURLcode trace_result = curl_global_trace(config);
            (void)trace_result; // Use to avoid unused variable warning
            free(config);
        }
    }
    
    // Clean up if we initialized
    if (init_result == CURLE_OK && init_type != 2) {
        curl_global_cleanup();
    }
    
    return 0;
}