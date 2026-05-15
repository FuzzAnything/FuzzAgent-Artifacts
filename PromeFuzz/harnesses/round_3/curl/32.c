// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
#define _GNU_SOURCE
#define CURL_DISABLE_TYPECHECK

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create dummy file if needed
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy);
        }
        fclose(dummy);
    }
    
    // Test 1: curl_multi_init and curl_multi_cleanup
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // Test 2: Create easy handles
        CURL *easy_handle1 = curl_easy_init();
        CURL *easy_handle2 = curl_easy_init();
        CURL *easy_handle3 = NULL;
        
        if (easy_handle1) {
            // Test 3: curl_multi_add_handle
            CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle1);
            (void)add_result;
            
            // Test 4: curl_multi_get_handles
            CURL **handles = curl_multi_get_handles(multi_handle);
            if (handles) {
                free(handles);
            }
            
            // Test 5: curl_multi_remove_handle
            CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle1);
            (void)remove_result;
            
            // Test 6: curl_easy_cleanup
            curl_easy_cleanup(easy_handle1);
        }
        
        if (easy_handle2) {
            // Add and remove multiple times to explore different states
            for (int i = 0; i < 3 && i < Size; i++) {
                curl_multi_add_handle(multi_handle, easy_handle2);
                if (i % 2 == 0) {
                    CURL **handles = curl_multi_get_handles(multi_handle);
                    if (handles) {
                        free(handles);
                    }
                }
                curl_multi_remove_handle(multi_handle, easy_handle2);
            }
            curl_easy_cleanup(easy_handle2);
        }
        
        // Test with NULL handles
        if (Size > 0 && (Data[0] % 2 == 0)) {
            curl_multi_add_handle(multi_handle, easy_handle3);
            curl_multi_remove_handle(multi_handle, easy_handle3);
        }
        
        // Test 7: curl_multi_cleanup
        CURLMcode cleanup_result = curl_multi_cleanup(multi_handle);
        (void)cleanup_result;
        
        // Test cleanup on already cleaned handle
        if (Size > 1 && (Data[1] % 2 == 0)) {
            curl_multi_cleanup(multi_handle);
        }
    }
    
    // Test with multiple multi handles
    if (Size > 2) {
        CURLM *multi1 = curl_multi_init();
        CURLM *multi2 = curl_multi_init();
        CURL *easy = curl_easy_init();
        
        if (multi1 && multi2 && easy) {
            // Add to first multi handle
            curl_multi_add_handle(multi1, easy);
            
            // Try adding to second (should work after removal)
            curl_multi_remove_handle(multi1, easy);
            curl_multi_add_handle(multi2, easy);
            
            // Get handles from both
            CURL **handles1 = curl_multi_get_handles(multi1);
            CURL **handles2 = curl_multi_get_handles(multi2);
            
            if (handles1) free(handles1);
            if (handles2) free(handles2);
            
            // Cleanup
            curl_multi_remove_handle(multi2, easy);
            curl_easy_cleanup(easy);
        }
        
        if (multi1) curl_multi_cleanup(multi1);
        if (multi2) curl_multi_cleanup(multi2);
    }
    
    // Cleanup global state
    curl_global_cleanup();
    
    return 0;
}