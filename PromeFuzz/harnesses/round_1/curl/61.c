// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Define CURL_DISABLE_TYPECHECK to avoid the typecheck macros */
#define CURL_DISABLE_TYPECHECK

#include "curl/multi.h"

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Initialize global state if needed
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create an easy handle to add to multi
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_multi_add_handle(multi_handle, easy);
    }

    // Use fuzz data to determine which function to test
    if (Size > 0) {
        uint8_t func_selector = Data[0] % 6;
        size_t data_offset = 1;
        
        // Extract parameters from fuzz data
        curl_socket_t sock = -1;
        int running = 0;
        int ev_bitmask = 0;
        void *sockp = NULL;
        
        if (Size > data_offset + sizeof(curl_socket_t)) {
            memcpy(&sock, Data + data_offset, sizeof(curl_socket_t));
            data_offset += sizeof(curl_socket_t);
        }
        
        if (Size > data_offset + sizeof(int)) {
            memcpy(&running, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
        }
        
        if (Size > data_offset + sizeof(int)) {
            memcpy(&ev_bitmask, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
        }
        
        if (Size > data_offset) {
            sockp = (void *)(Data + data_offset);
        }

        switch (func_selector) {
            case 0: {
                // Test curl_multi_setopt with various options
                CURLMoption options[] = {
                    CURLMOPT_PIPELINING,
                    CURLMOPT_MAXCONNECTS,
                    CURLMOPT_MAX_TOTAL_CONNECTIONS,
                    CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE,
                    CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE,
                    CURLMOPT_PIPELINING_SITE_BL,
                    CURLMOPT_PIPELINING_SERVER_BL,
                    CURLMOPT_MAX_CONCURRENT_STREAMS
                };
                
                size_t opt_idx = 0;
                if (Size > 1) {
                    opt_idx = Data[1] % (sizeof(options) / sizeof(options[0]));
                }
                
                // Use variadic parameter based on option type
                CURLMoption selected_opt = options[opt_idx];
                long param = 0;
                if (Size > 2) {
                    param = Data[2];
                }
                
                // Set callback options to NULL or dummy functions
                if (selected_opt == CURLMOPT_SOCKETFUNCTION || 
                    selected_opt == CURLMOPT_TIMERFUNCTION) {
                    curl_multi_setopt(multi_handle, selected_opt, NULL);
                } else {
                    curl_multi_setopt(multi_handle, selected_opt, param);
                }
                break;
            }
            
            case 1:
                // Test curl_multi_socket (deprecated)
                if (sock != -1) {
                    curl_multi_socket(multi_handle, sock, &running);
                }
                break;
                
            case 2:
                // Test curl_multi_assign
                if (sock != -1) {
                    curl_multi_assign(multi_handle, sock, sockp);
                }
                break;
                
            case 3:
                // Test curl_multi_socket_all (deprecated)
                curl_multi_socket_all(multi_handle, &running);
                break;
                
            case 4:
                // Test curl_multi_perform
                curl_multi_perform(multi_handle, &running);
                break;
                
            case 5:
                // Test curl_multi_socket_action
                if (sock != -1) {
                    curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running);
                }
                break;
        }
    }

    // Clean up
    if (easy) {
        curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi_handle);
    
    return 0;
}