// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Write dummy file for potential file operations
    write_dummy_file(Data, Size);
    
    // Use fuzzer data to determine which options to set
    // We'll use the first byte to select an option category
    uint8_t option_category = Data[0];
    
    // Set various options based on fuzzer input
    // We'll try to cover different parameter types
    size_t data_offset = 1;
    
    // String options
    if (option_category % 4 == 0 && Size > data_offset) {
        size_t str_len = Size - data_offset;
        if (str_len > 0) {
            char *url = malloc(str_len + 1);
            if (url) {
                memcpy(url, Data + data_offset, str_len);
                url[str_len] = '\0';
                curl_easy_setopt(curl, CURLOPT_URL, url);
                free(url);
            }
        }
    }
    
    // Long options
    if (option_category % 4 == 1 && Size >= data_offset + sizeof(long)) {
        long timeout = 0;
        memcpy(&timeout, Data + data_offset, sizeof(long));
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        data_offset += sizeof(long);
    }
    
    // Function pointer options (dummy callbacks)
    if (option_category % 4 == 2) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    }
    
    // Object pointer options
    if (option_category % 4 == 3 && Size > data_offset) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (void*)(Data + data_offset));
        size_t postfield_size = Size - data_offset;
        if (postfield_size > 0) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postfield_size);
        }
    }
    
    // Set some common options
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    // Try to perform the transfer
    // This will likely fail without a real server, but we want to exercise the code
    CURLcode res = curl_easy_perform(curl);
    
    // Test duphandle
    CURL *duplicate = curl_easy_duphandle(curl);
    if (duplicate) {
        // Set some options on the duplicate
        curl_easy_setopt(duplicate, CURLOPT_VERBOSE, 0L);
        
        // Try to perform with duplicate
        CURLcode dup_res = curl_easy_perform(duplicate);
        
        // Test send on duplicate if we had CONNECT_ONLY mode
        if (option_category % 2 == 0) {
            size_t sent = 0;
            if (Size > 10) {
                curl_easy_send(duplicate, Data, Size > 1024 ? 1024 : Size, &sent);
            }
        }
        
        curl_easy_cleanup(duplicate);
    }
    
    // Test reset
    curl_easy_reset(curl);
    
    // Set some options after reset
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    
    // Try send on original handle (will likely fail without CONNECT_ONLY)
    if (option_category % 3 == 0 && Size > 5) {
        size_t sent = 0;
        curl_easy_send(curl, Data, Size > 512 ? 512 : Size, &sent);
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}