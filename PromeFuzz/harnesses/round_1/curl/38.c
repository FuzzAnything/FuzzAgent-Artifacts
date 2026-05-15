// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version_info at version.c:596:25 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_version at version.c:145:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Custom memory callbacks for curl_global_init_mem */
static void *my_malloc_callback(size_t size) {
    return malloc(size);
}

static void my_free_callback(void *ptr) {
    free(ptr);
}

static void *my_realloc_callback(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *my_strdup_callback(const char *str) {
    return strdup(str);
}

static void *my_calloc_callback(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    /* Use first byte to decide which initialization function to test */
    uint8_t init_choice = 0;
    if (Size > 0) {
        init_choice = Data[0] % 3;
        Data++;
        Size--;
    }
    
    /* Prepare flags from fuzzer data */
    long flags = CURL_GLOBAL_DEFAULT;
    if (Size >= sizeof(long)) {
        memcpy(&flags, Data, sizeof(long));
        Data += sizeof(long);
        Size -= sizeof(long);
        /* Ensure flags are valid - mask with known flag values */
        flags &= (CURL_GLOBAL_SSL | CURL_GLOBAL_WIN32 | CURL_GLOBAL_ALL | 
                  CURL_GLOBAL_NOTHING | CURL_GLOBAL_DEFAULT);
    }
    
    /* Initialize curl with different methods based on fuzzer input */
    CURLcode init_result = CURLE_OK;
    switch (init_choice) {
        case 0:
            init_result = curl_global_init(flags);
            break;
        case 1:
            init_result = curl_global_init_mem(flags,
                my_malloc_callback,
                my_free_callback,
                my_realloc_callback,
                my_strdup_callback,
                my_calloc_callback);
            break;
        case 2:
            /* Test with different flag combinations */
            init_result = curl_global_init(CURL_GLOBAL_ALL);
            if (init_result != CURLE_OK) {
                init_result = curl_global_init(CURL_GLOBAL_NOTHING);
            }
            break;
    }
    
    /* If initialization succeeded, test other functions */
    if (init_result == CURLE_OK) {
        /* Test curl_version - always safe to call */
        const char *version_str = curl_version();
        (void)version_str; /* Use result to avoid unused variable warning */
        
        /* Test curl_version_info with different version requests */
        const curl_version_info_data *info = NULL;
        CURLversion version_requests[] = {
            CURLVERSION_FIRST,
            CURLVERSION_SECOND,
            CURLVERSION_THIRD,
            CURLVERSION_FOURTH,
            CURLVERSION_FIFTH,
            CURLVERSION_SIXTH,
            CURLVERSION_SEVENTH,
            CURLVERSION_EIGHTH,
            CURLVERSION_NINTH,
            CURLVERSION_TENTH,
            CURLVERSION_ELEVENTH,
            CURLVERSION_TWELFTH,
            CURLVERSION_NOW
        };
        
        size_t num_versions = sizeof(version_requests) / sizeof(version_requests[0]);
        if (Size > 0) {
            size_t idx = Data[0] % num_versions;
            info = curl_version_info(version_requests[idx]);
        } else {
            info = curl_version_info(CURLVERSION_NOW);
        }
        (void)info; /* Use result to avoid unused variable warning */
        
        /* Test curl_global_trace with fuzzer-provided config string */
        if (Size > 1) {
            /* Create null-terminated config string from fuzzer data */
            size_t config_len = Size;
            if (config_len > 1024) config_len = 1024; /* Limit size */
            char *config = (char*)malloc(config_len + 1);
            if (config) {
                memcpy(config, Data, config_len);
                config[config_len] = '\0';
                
                /* Try different trace configurations */
                CURLcode trace_result = curl_global_trace(config);
                (void)trace_result; /* Use result to avoid unused variable warning */
                
                /* Also test with common trace configurations */
                if (config_len > 0) {
                    curl_global_trace("http/2");
                    curl_global_trace("http/3");
                    curl_global_trace("all");
                }
                
                free(config);
            }
        } else {
            /* Test with empty string */
            curl_global_trace("");
        }
    }
    
    /* Always clean up if we initialized */
    if (init_result == CURLE_OK) {
        curl_global_cleanup();
        
        /* Test that we can re-initialize after cleanup */
        if (Size > 1) {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl_version();
            curl_global_cleanup();
        }
    }
    
    return 0;
}