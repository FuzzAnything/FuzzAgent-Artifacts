// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for potential use
    write_dummy_file(Data, Size);
    
    // Test curl_easy_option_by_id with various IDs
    for (int i = 0; i < 256; i++) {
        const struct curl_easyoption *opt = curl_easy_option_by_id((CURLoption)i);
        if (opt) {
            // Option exists, we can optionally use its type information
            curl_easytype type = opt->type;
            (void)type; // Suppress unused variable warning
        }
    }
    
    // Test curl_easy_option_next iteration
    const struct curl_easyoption *opt = NULL;
    int option_count = 0;
    while ((opt = curl_easy_option_next(opt)) != NULL && option_count < 100) {
        option_count++;
        
        // Test curl_easy_option_by_name for each option found
        if (opt->name) {
            const struct curl_easyoption *found = curl_easy_option_by_name(opt->name);
            (void)found; // Suppress unused variable warning
        }
    }
    
    // Test curl_easy_option_by_name with random strings from fuzzer input
    if (Size > 0) {
        // Create null-terminated string from fuzzer data (limited to 100 chars)
        char name_buf[101];
        size_t name_len = Size < 100 ? Size : 100;
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        
        const struct curl_easyoption *named_opt = curl_easy_option_by_name(name_buf);
        (void)named_opt; // Suppress unused variable warning
    }
    
    // Initialize curl handle for further testing
    CURL *curl = curl_easy_init();
    if (curl) {
        // Set some basic options using fuzzer data
        if (Size > 0) {
            // Use first byte to determine some option values
            long verbose = Data[0] & 1;
            curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
            
            // Set URL to dummy file
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            
            // Set various other options based on fuzzer data
            if (Size > 1) {
                long header = Data[1] & 1;
                curl_easy_setopt(curl, CURLOPT_HEADER, header);
            }
            
            if (Size > 2) {
                long nobody = Data[2] & 1;
                curl_easy_setopt(curl, CURLOPT_NOBODY, nobody);
            }
            
            // Try to set a string option if we have enough data
            if (Size > 10) {
                // Create null-terminated useragent string
                char useragent[101];
                size_t ua_len = (Size - 3) < 100 ? (Size - 3) : 100;
                memcpy(useragent, Data + 3, ua_len);
                useragent[ua_len] = '\0';
                curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);
            }
        }
        
        // Perform transfer (may fail, but that's OK for fuzzing)
        CURLcode res = curl_easy_perform(curl);
        
        // Test curl_easy_getinfo after perform
        if (res == CURLE_OK) {
            double total_time;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            
            double size_download;
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
        }
        
        // Clean up curl handle
        curl_easy_cleanup(curl);
    }
    
    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}