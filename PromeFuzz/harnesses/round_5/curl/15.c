// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/options.h"

static void dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Dummy write callback to consume response data
    (void)ptr;
    (void)userdata;
}

static CURLcode perform_dummy_transfer(CURL *curl) {
    // Set up a dummy transfer that won't actually connect anywhere
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9999/dummy");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    
    return curl_easy_perform(curl);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize curl globally (idempotent)
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Use first byte to determine which functions to exercise
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Exercise curl_easy_option_by_name with fuzz data as option name
    if (Size > data_offset) {
        // Create null-terminated string from fuzz data
        size_t name_len = Size - data_offset;
        if (name_len > 255) name_len = 255;
        char *option_name = (char*)malloc(name_len + 1);
        if (option_name) {
            memcpy(option_name, Data + data_offset, name_len);
            option_name[name_len] = '\0';
            
            const struct curl_easyoption *opt = curl_easy_option_by_name(option_name);
            (void)opt; // Result may be NULL, that's fine
            
            free(option_name);
        }
    }
    
    // Exercise curl_easy_option_next to iterate through options
    const struct curl_easyoption *opt = NULL;
    int iterations = selector % 10; // Limit iterations
    for (int i = 0; i < iterations; i++) {
        opt = curl_easy_option_next(opt);
        if (!opt) break;
        
        // Exercise curl_easy_option_by_id with the found option ID
        const struct curl_easyoption *opt_by_id = curl_easy_option_by_id(opt->id);
        (void)opt_by_id;
    }
    
    // Exercise curl_easy_setopt with various option types
    if (Size > data_offset + 4) {
        // Try setting some common options with fuzz data
        uint32_t int_val;
        memcpy(&int_val, Data + data_offset, sizeof(int_val));
        
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(int_val & 1));
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, (long)((int_val >> 1) & 1));
        
        // Try setting a string option if we have enough data
        if (Size > data_offset + 8) {
            size_t str_len = (Size - data_offset - 4) % 128;
            char *dummy_str = (char*)malloc(str_len + 1);
            if (dummy_str) {
                memcpy(dummy_str, Data + data_offset + 4, str_len);
                dummy_str[str_len] = '\0';
                
                curl_easy_setopt(curl, CURLOPT_USERAGENT, dummy_str);
                free(dummy_str);
            }
        }
    }
    
    // Perform a dummy transfer (will likely fail, but that's OK)
    CURLcode perform_res = perform_dummy_transfer(curl);
    (void)perform_res;
    
    // Exercise curl_easy_getinfo after the dummy transfer
    if (selector & 0x80) {
        // Try getting different types of info
        double time_val;
        long long_val;
        const char *str_val;
        curl_off_t off_val;
        
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_val);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &str_val);
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_val);
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}