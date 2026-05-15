// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static CURLM *multi_handle = NULL;
static CURL *easy_handle = NULL;

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Initialize multi handle if not already done
    if (multi_handle == NULL) {
        multi_handle = curl_multi_init();
        if (multi_handle == NULL) {
            return 0;
        }
    }
    
    // Initialize easy handle if not already done
    if (easy_handle == NULL) {
        easy_handle = curl_easy_init();
        if (easy_handle == NULL) {
            curl_multi_cleanup(multi_handle);
            multi_handle = NULL;
            return 0;
        }
    }
    
    // Use fuzzer input to create variations
    int operation = 0;
    if (Size > 0) {
        operation = Data[0] % 6;
    }
    
    // Perform different operations based on fuzzer input
    switch (operation) {
        case 0: {
            // Test curl_multi_cleanup with valid handle
            CURLMcode cleanup_result = curl_multi_cleanup(multi_handle);
            // Reinitialize for next iteration
            multi_handle = curl_multi_init();
            break;
        }
        case 1: {
            // Test curl_multi_add_handle
            CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
            
            // If successful, test removal
            if (add_result == CURLM_OK) {
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        }
        case 2: {
            // Test curl_easy_cleanup
            curl_easy_cleanup(easy_handle);
            // Reinitialize for next iteration
            easy_handle = curl_easy_init();
            break;
        }
        case 3: {
            // Test curl_multi_get_handles
            CURL **handles = curl_multi_get_handles(multi_handle);
            if (handles != NULL) {
                free(handles);
            }
            break;
        }
        case 4: {
            // Test curl_multi_remove_handle (without prior add)
            CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
            break;
        }
        case 5: {
            // Test multiple operations in sequence
            CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
            if (add_result == CURLM_OK) {
                CURL **handles = curl_multi_get_handles(multi_handle);
                if (handles != NULL) {
                    free(handles);
                }
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        }
    }
    
    // Test edge cases with NULL handles
    if (Size > 1 && (Data[1] % 2) == 0) {
        // Test with NULL multi handle
        if (Size > 2 && (Data[2] % 2) == 0) {
            curl_multi_cleanup(NULL);
        }
        
        // Test with NULL easy handle
        if (Size > 3 && (Data[3] % 2) == 0) {
            curl_easy_cleanup(NULL);
        }
        
        // Test add with NULL handles
        if (Size > 4 && (Data[4] % 2) == 0) {
            curl_multi_add_handle(NULL, easy_handle);
        }
        if (Size > 5 && (Data[5] % 2) == 0) {
            curl_multi_add_handle(multi_handle, NULL);
        }
        
        // Test remove with NULL handles
        if (Size > 6 && (Data[6] % 2) == 0) {
            curl_multi_remove_handle(NULL, easy_handle);
        }
        if (Size > 7 && (Data[7] % 2) == 0) {
            curl_multi_remove_handle(multi_handle, NULL);
        }
        
        // Test get_handles with NULL
        if (Size > 8 && (Data[8] % 2) == 0) {
            curl_multi_get_handles(NULL);
        }
    }
    
    // Clean up and reinitialize handles periodically to test different states
    if (Size > 9 && (Data[9] % 10) == 0) {
        if (multi_handle != NULL) {
            curl_multi_cleanup(multi_handle);
            multi_handle = curl_multi_init();
        }
        if (easy_handle != NULL) {
            curl_easy_cleanup(easy_handle);
            easy_handle = curl_easy_init();
        }
    }
    
    return 0;
}