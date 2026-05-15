// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Disable typechecking to avoid compilation errors with curl_multi_setopt macro */
#define CURL_DISABLE_TYPECHECK

#include "curl/multi.h"
#include "curl/easy.h"

static void cleanup_multi_handle(CURLM *multi_handle) {
    if (multi_handle) {
        // Remove all easy handles before cleanup
        CURL **handles = curl_multi_get_handles(multi_handle);
        if (handles) {
            for (int i = 0; handles[i] != NULL; i++) {
                curl_multi_remove_handle(multi_handle, handles[i]);
                curl_easy_cleanup(handles[i]);
            }
            free(handles);
        }
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to determine fuzzing mode
    uint8_t mode = Data[0];
    size_t offset = 1;
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    // Create some easy handles based on fuzzer input
    CURL *easy_handles[5] = {NULL};
    int num_easy_handles = 0;
    
    // Use fuzzer data to determine number of easy handles (1-5)
    if (offset < Size) {
        num_easy_handles = (Data[offset] % 5) + 1;
        offset++;
    }
    
    for (int i = 0; i < num_easy_handles && i < 5; i++) {
        easy_handles[i] = curl_easy_init();
        if (!easy_handles[i]) {
            // Clean up already created handles
            for (int j = 0; j < i; j++) {
                curl_easy_cleanup(easy_handles[j]);
            }
            curl_multi_cleanup(multi_handle);
            return 0;
        }
    }
    
    // Mode 0: Test basic add/remove operations
    if (mode % 3 == 0) {
        for (int i = 0; i < num_easy_handles; i++) {
            if (easy_handles[i]) {
                curl_multi_add_handle(multi_handle, easy_handles[i]);
            }
        }
        
        // Test get_handles
        CURL **handles = curl_multi_get_handles(multi_handle);
        if (handles) {
            free(handles);
        }
        
        // Remove some handles based on fuzzer input
        if (offset < Size) {
            int remove_count = Data[offset] % (num_easy_handles + 1);
            offset++;
            
            for (int i = 0; i < remove_count && i < num_easy_handles; i++) {
                if (easy_handles[i]) {
                    curl_multi_remove_handle(multi_handle, easy_handles[i]);
                }
            }
        }
    }
    // Mode 1: Test repeated add/remove operations
    else if (mode % 3 == 1) {
        // Perform multiple add/remove cycles based on fuzzer data
        int cycles = 3;
        if (offset < Size) {
            cycles = (Data[offset] % 5) + 1;
            offset++;
        }
        
        for (int cycle = 0; cycle < cycles; cycle++) {
            for (int i = 0; i < num_easy_handles; i++) {
                if (easy_handles[i]) {
                    curl_multi_add_handle(multi_handle, easy_handles[i]);
                }
            }
            
            // Test get_handles in each cycle
            CURL **handles = curl_multi_get_handles(multi_handle);
            if (handles) {
                free(handles);
            }
            
            for (int i = 0; i < num_easy_handles; i++) {
                if (easy_handles[i]) {
                    curl_multi_remove_handle(multi_handle, easy_handles[i]);
                }
            }
        }
    }
    // Mode 2: Test partial operations and cleanup
    else {
        // Add only some handles
        int add_count = num_easy_handles / 2;
        if (offset < Size) {
            add_count = Data[offset] % (num_easy_handles + 1);
            offset++;
        }
        
        for (int i = 0; i < add_count && i < num_easy_handles; i++) {
            if (easy_handles[i]) {
                curl_multi_add_handle(multi_handle, easy_handles[i]);
            }
        }
        
        // Test get_handles
        CURL **handles = curl_multi_get_handles(multi_handle);
        if (handles) {
            free(handles);
        }
    }
    
    // Clean up all easy handles
    for (int i = 0; i < num_easy_handles; i++) {
        if (easy_handles[i]) {
            // Remove from multi handle if still added
            curl_multi_remove_handle(multi_handle, easy_handles[i]);
            curl_easy_cleanup(easy_handles[i]);
        }
    }
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    return 0;
}