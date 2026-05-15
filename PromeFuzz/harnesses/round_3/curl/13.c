// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy memory callbacks for curl_global_init_mem */
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
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char *copy = (char*)malloc(len);
    if (copy) memcpy(copy, str, len);
    return copy;
}

static void *dummy_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    /* Ensure we have at least some data for string operations */
    if (Size < 1) return 0;
    
    /* Create null-terminated string from fuzzer data for curl_global_trace */
    char *trace_config = (char*)malloc(Size + 1);
    if (!trace_config) return 0;
    memcpy(trace_config, Data, Size);
    trace_config[Size] = '\0';
    
    /* Try different initialization patterns */
    static int init_counter = 0;
    CURLcode init_res;
    
    /* Alternate between curl_global_init and curl_global_init_mem */
    if (init_counter % 3 == 0) {
        /* Use fuzzer data to determine flags (mask to valid flag bits) */
        long flags = (long)(Data[0] & 0x3); /* CURL_GLOBAL_* flags */
        init_res = curl_global_init(flags);
    } else if (init_counter % 3 == 1) {
        long flags = (long)(Data[0] & 0x3);
        init_res = curl_global_init_mem(flags, 
                                       dummy_malloc, 
                                       dummy_free, 
                                       dummy_realloc, 
                                       dummy_strdup, 
                                       dummy_calloc);
    } else {
        /* Try with default flags */
        init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    
    init_counter++;
    
    /* Only proceed if initialization succeeded */
    if (init_res == CURLE_OK) {
        /* Try curl_global_trace with fuzzer-provided config string */
        curl_global_trace(trace_config);
        
        /* Get version info with different version requests */
        curl_version_info_data *info;
        if (Size % 4 == 0) {
            info = curl_version_info(CURLVERSION_NOW);
        } else if (Size % 4 == 1) {
            info = curl_version_info(CURLVERSION_FIRST);
        } else if (Size % 4 == 2) {
            info = curl_version_info(CURLVERSION_SECOND);
        } else {
            info = curl_version_info(CURLVERSION_THIRD);
        }
        
        /* Try curl_global_sslset with different parameters */
        const curl_ssl_backend ***avail = NULL;
        CURLsslset sslset_res;
        
        if (Size > 1) {
            /* Try with ID from fuzzer data */
            curl_sslbackend id = (curl_sslbackend)(Data[1] % 10); /* Limit to valid range */
            sslset_res = curl_global_sslset(id, NULL, avail);
            
            /* Also try with name */
            if (Size > 10) {
                char ssl_name[10];
                memcpy(ssl_name, Data, 9);
                ssl_name[9] = '\0';
                sslset_res = curl_global_sslset(CURLSSLBACKEND_NONE, ssl_name, avail);
            }
        }
        
        /* Clean up */
        curl_global_cleanup();
    }
    
    free(trace_config);
    return 0;
}