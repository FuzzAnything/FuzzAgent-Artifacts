// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write minimal data if empty input
            const uint8_t dummy[] = {0};
            fwrite(dummy, 1, 1, fp);
        }
        fclose(fp);
    }
    
    // Initialize curl handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Set various options using fuzzer data
    if (Size > 0) {
        // Use first byte to select option type
        uint8_t opt_type = Data[0];
        
        // Set URL option
        char url[] = "file://./dummy_file";
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        // Set other options based on fuzzer input
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(opt_type & 1));
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, (long)((opt_type >> 1) & 1));
        
        // Set buffer for potential receive operations
        if (Size > 1) {
            char *post_data = (char *)malloc(Size);
            if (post_data) {
                memcpy(post_data, Data, Size);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)Size);
                free(post_data);
            }
        }
    }
    
    // Perform transfer
    CURLcode res = curl_easy_perform(curl);
    
    // Try receive operation if perform succeeded
    if (res == CURLE_OK) {
        char recv_buf[256];
        size_t n = 0;
        curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n);
    }
    
    // Reset handle
    curl_easy_reset(curl);
    
    // Reconfigure and perform again
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_perform(curl);
    
    // Duplicate handle
    CURL *dup_curl = curl_easy_duphandle(curl);
    if (dup_curl) {
        curl_easy_setopt(dup_curl, CURLOPT_URL, "file://./dummy_file");
        curl_easy_perform(dup_curl);
        curl_easy_cleanup(dup_curl);
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}