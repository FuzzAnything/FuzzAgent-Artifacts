// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"

/* Dummy memory callbacks for curl_global_init_mem */
static void *dummy_malloc_callback(size_t size) {
    return malloc(size);
}

static void dummy_free_callback(void *ptr) {
    free(ptr);
}

static void *dummy_realloc_callback(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *dummy_strdup_callback(const char *str) {
    return strdup(str);
}

static void *dummy_calloc_callback(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

/* Helper function to write data to dummy file */
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    /* Generate values from fuzzer input */
    long flags = 0;
    curl_sslbackend id = (curl_sslbackend)0;
    const char *name = NULL;
    const char *trace_config = NULL;
    
    if (Size > 0) {
        /* Use first byte for flags/ID selection */
        flags = (long)Data[0];
        id = (curl_sslbackend)Data[0];
        
        /* Use remaining data for strings if available */
        if (Size > 1) {
            /* Ensure null-terminated string for name */
            size_t name_len = Size > 256 ? 256 : Size - 1;
            static char name_buf[257];
            memcpy(name_buf, Data + 1, name_len);
            name_buf[name_len] = '\0';
            name = name_buf;
            
            /* Use same buffer for trace config */
            trace_config = name_buf;
        }
    }
    
    /* Test curl_global_sslset */
    const curl_ssl_backend ***avail = NULL;
    curl_global_sslset(id, name, avail);
    
    /* Test curl_global_init */
    CURLcode init_result = curl_global_init(flags);
    
    /* Test curl_global_init_mem - only if global init hasn't been called yet */
    if (init_result != CURLE_OK) {
        curl_global_init_mem(flags,
                            dummy_malloc_callback,
                            dummy_free_callback,
                            dummy_realloc_callback,
                            dummy_strdup_callback,
                            dummy_calloc_callback);
    }
    
    /* Test curl_global_trace */
    curl_global_trace(trace_config);
    
    /* Create and cleanup multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        /* Test curl_multi_cleanup with valid handle */
        curl_multi_cleanup(multi_handle);
        
        /* Test curl_multi_cleanup with NULL handle */
        if (Size % 2 == 0) {
            curl_multi_cleanup(NULL);
        }
    }
    
    /* Test curl_global_cleanup */
    curl_global_cleanup();
    
    /* Create dummy file for any potential file-based operations */
    write_dummy_file(Data, Size);
    
    return 0;
}