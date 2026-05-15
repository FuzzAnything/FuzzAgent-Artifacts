// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "urlapi.h"
#include "curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_url_operations(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create initial CURLU handle
    CURLU *url = curl_url();
    if (!url) return;
    
    // Use first byte to determine operation mix
    uint8_t op_selector = data[0];
    size_t data_offset = 1;
    
    // Set some URL parts using fuzzer data
    if (size > data_offset + 10) {
        // Create null-terminated strings from fuzzer data
        char scheme[3] = {0};
        char host[11] = {0};
        char path[11] = {0};
        
        size_t scheme_len = size - data_offset > 2 ? 2 : size - data_offset;
        memcpy(scheme, data + data_offset, scheme_len);
        scheme[scheme_len] = '\0';
        data_offset += scheme_len;
        
        if (size > data_offset + 10) {
            size_t host_len = size - data_offset > 10 ? 10 : size - data_offset;
            memcpy(host, data + data_offset, host_len);
            host[host_len] = '\0';
            data_offset += host_len;
            
            if (size > data_offset + 10) {
                size_t path_len = size - data_offset > 10 ? 10 : size - data_offset;
                memcpy(path, data + data_offset, path_len);
                path[path_len] = '\0';
                data_offset += path_len;
                
                // Set URL components
                curl_url_set(url, CURLUPART_SCHEME, scheme, 0);
                curl_url_set(url, CURLUPART_HOST, host, 0);
                curl_url_set(url, CURLUPART_PATH, path, 0);
            }
        }
    }
    
    // Test curl_url_dup
    if (op_selector & 0x01) {
        CURLU *dup_url = curl_url_dup(url);
        if (dup_url) {
            // Test curl_url_get on duplicate
            char *part = NULL;
            CURLUcode get_result = curl_url_get(dup_url, CURLUPART_SCHEME, &part, 0);
            if (get_result == CURLUE_OK && part) {
                curl_free(part);
            }
            
            // Test error string function for URL operations
            const char *err_str = curl_url_strerror(get_result);
            (void)err_str; // Use to avoid unused variable warning
            
            curl_url_cleanup(dup_url);
        }
    }
    
    // Test curl_url_get on original handle
    if (op_selector & 0x02) {
        char *part = NULL;
        CURLUPart what_part = (op_selector >> 1) % 6; // Cycle through URL parts
        
        CURLUcode get_result = curl_url_get(url, what_part, &part, 0);
        if (get_result == CURLUE_OK && part) {
            curl_free(part);
        }
        
        // Test URL error string function
        const char *url_err = curl_url_strerror(get_result);
        (void)url_err;
    }
    
    // Test error string functions with fuzzer-derived error codes
    if (op_selector & 0x04) {
        // Use remaining data to generate error codes
        for (size_t i = data_offset; i + 1 < size; i += 2) {
            CURLSHcode share_code = (CURLSHcode)(data[i] % 10); // Limited range
            CURLcode easy_code = (CURLcode)(data[i+1] % 100); // Limited range
            
            const char *share_err = curl_share_strerror(share_code);
            const char *easy_err = curl_easy_strerror(easy_code);
            
            (void)share_err;
            (void)easy_err;
        }
    }
    
    // Clean up original handle
    curl_url_cleanup(url);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *data, size_t size) {
    fuzz_url_operations(data, size);
    return 0;
}