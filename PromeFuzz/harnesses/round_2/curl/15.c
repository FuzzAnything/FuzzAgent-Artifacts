// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"
#include "easy.h"
#include "options.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential use
    write_dummy_file(Data, Size);
    
    // Test curl_easy_option_by_id with various IDs
    for (CURLoption id = 0; id < 1000; id += 100) {
        const struct curl_easyoption *opt = curl_easy_option_by_id(id);
        if (opt) {
            // Option found, check its properties
            (void)opt->name;
            (void)opt->type;
            (void)opt->flags;
        }
    }
    
    // Test curl_easy_option_by_name with various names
    if (Size > 0) {
        // Use first part of input as option name (null-terminated)
        size_t name_len = Size > 100 ? 100 : Size;
        char *name = malloc(name_len + 1);
        if (name) {
            memcpy(name, Data, name_len);
            name[name_len] = '\0';
            const struct curl_easyoption *opt = curl_easy_option_by_name(name);
            if (opt) {
                (void)opt->id;
                (void)opt->type;
            }
            free(name);
        }
    }
    
    // Test curl_easy_option_next iteration
    const struct curl_easyoption *opt = NULL;
    int count = 0;
    while ((opt = curl_easy_option_next(opt)) != NULL && count < 50) {
        (void)opt->name;
        (void)opt->id;
        count++;
    }
    
    // Test curl_easy_setopt and curl_easy_perform with a handle
    CURL *curl = curl_easy_init();
    if (curl) {
        // Set some common options
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        
        // Set options based on fuzzer input
        if (Size > 0) {
            // Use input data to set various option types
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (Size & 1) ? 1L : 0L);
            
            // Set a file upload if we have data
            if (Size > 10) {
                FILE *fp = fopen("./dummy_file", "rb");
                if (fp) {
                    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(curl, CURLOPT_READDATA, fp);
                    curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)Size);
                    
                    // Try to perform the transfer
                    CURLcode res = curl_easy_perform(curl);
                    (void)res;
                    
                    fclose(fp);
                }
            } else {
                // Just perform without upload
                CURLcode res = curl_easy_perform(curl);
                (void)res;
            }
        }
        
        // Test curl_easy_getinfo after potential transfer
        if (Size > 0) {
            double time_val;
            CURLcode info_res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_val);
            (void)info_res;
            
            long response_code;
            info_res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            (void)info_res;
            
            char *content_type = NULL;
            info_res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
            (void)info_res;
        }
        
        curl_easy_cleanup(curl);
    }
    
    // Clean up dummy file
    cleanup_dummy_file();
    
    return 0;
}