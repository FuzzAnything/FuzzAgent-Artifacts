// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to decide which path to take
    uint8_t selector = Data[0] % 6;
    Data++;
    Size--;

    switch (selector) {
        case 0: {
            // Test curl_global_init
            long flags = 0;
            if (Size >= sizeof(long)) {
                flags = *(const long *)Data;
            }
            CURLcode res = curl_global_init(flags);
            if (res == CURLE_OK) {
                curl_global_cleanup();
            }
            break;
        }
        case 1: {
            // Test curl_global_cleanup (after init)
            long flags = 0;
            if (Size >= sizeof(long)) {
                flags = *(const long *)Data;
            }
            CURLcode res = curl_global_init(flags);
            if (res == CURLE_OK) {
                curl_global_cleanup();
            }
            break;
        }
        case 2: {
            // Test curl_global_init_mem
            long flags = 0;
            if (Size >= sizeof(long)) {
                flags = *(const long *)Data;
            }
            
            // Use dummy memory callbacks
            CURLcode res = curl_global_init_mem(flags,
                                                dummy_malloc,
                                                dummy_free,
                                                dummy_realloc,
                                                dummy_strdup,
                                                dummy_calloc);
            if (res == CURLE_OK) {
                curl_global_cleanup();
            }
            break;
        }
        case 3: {
            // Test curl_version
            char *version = curl_version();
            (void)version; // Use to avoid unused variable warning
            break;
        }
        case 4: {
            // Test curl_global_trace
            if (Size > 0) {
                // Create null-terminated string from fuzzer data
                char *config = (char *)malloc(Size + 1);
                if (config) {
                    memcpy(config, Data, Size);
                    config[Size] = '\0';
                    
                    // Initialize before trace
                    curl_global_init(0);
                    curl_global_trace(config);
                    curl_global_cleanup();
                    
                    free(config);
                }
            }
            break;
        }
        case 5: {
            // Test curl_version_info
            CURLversion stamp = CURLVERSION_NOW;
            if (Size >= sizeof(CURLversion)) {
                stamp = *(const CURLversion *)Data;
                // Ensure it's a valid version constant
                if (stamp > CURLVERSION_TWELFTH) {
                    stamp = CURLVERSION_NOW;
                }
            }
            const curl_version_info_data *info = curl_version_info(stamp);
            (void)info; // Use to avoid unused variable warning
            break;
        }
    }

    return 0;
}