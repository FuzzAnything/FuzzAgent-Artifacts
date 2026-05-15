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
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"

/* Memory callbacks for curl_global_init_mem */
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

/* Array of known CURLversion values */
static const CURLversion version_stamps[] = {
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    /* Use first byte to determine which initialization to use */
    uint8_t init_choice = 0;
    if (Size > 0) {
        init_choice = Data[0] & 0x01;
    }

    /* Initialize libcurl */
    CURLcode init_result;
    if (init_choice == 0) {
        /* Use standard initialization */
        init_result = curl_global_init(CURL_GLOBAL_ALL);
    } else {
        /* Use memory callback initialization */
        init_result = curl_global_init_mem(
            CURL_GLOBAL_ALL,
            my_malloc_callback,
            my_free_callback,
            my_realloc_callback,
            my_strdup_callback,
            my_calloc_callback
        );
    }

    if (init_result != CURLE_OK) {
        /* Initialization failed, skip further processing */
        return 0;
    }

    /* Test curl_version */
    const char *version_str = curl_version();
    (void)version_str; /* Use the result to avoid unused variable warning */

    /* Test curl_version_info with different version stamps */
    size_t num_stamps = sizeof(version_stamps) / sizeof(version_stamps[0]);
    for (size_t i = 0; i < num_stamps; i++) {
        const curl_version_info_data *info = curl_version_info(version_stamps[i]);
        (void)info; /* Use the result to avoid unused variable warning */
    }

    /* Test curl_global_trace with fuzzer-provided config string */
    if (Size > 1) {
        /* Use Data+1 as the config string, ensure null termination */
        size_t config_len = Size - 1;
        if (config_len > 1024) {
            config_len = 1024; /* Limit size to avoid excessive allocation */
        }
        
        char *config = (char *)malloc(config_len + 1);
        if (config) {
            memcpy(config, Data + 1, config_len);
            config[config_len] = '\0';
            
            CURLcode trace_result = curl_global_trace(config);
            (void)trace_result; /* Use the result to avoid unused variable warning */
            
            free(config);
        }
    }

    /* Clean up */
    curl_global_cleanup();

    return 0;
}