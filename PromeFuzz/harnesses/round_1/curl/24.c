// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK 1
#include "multi.h"

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
        }
        close(fd);
    }

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Use fuzz data to determine which function to test
    if (Size > 0) {
        uint8_t func_selector = Data[0] % 6;
        const uint8_t *params = Data + 1;
        size_t params_size = Size - 1;
        
        // Common variables used by multiple functions
        int running_handles = 0;
        curl_socket_t sockfd = 0;
        void *sockp = NULL;
        
        // Extract parameters from fuzz data if available
        if (params_size >= sizeof(curl_socket_t)) {
            memcpy(&sockfd, params, sizeof(curl_socket_t));
        }
        if (params_size >= sizeof(void*)) {
            // Use a valid pointer (address of a local variable) for sockp
            sockp = &running_handles;
        }
        
        switch (func_selector) {
            case 0: {
                // Test curl_multi_setopt
                // Use a few known valid CURLMoption values
                CURLMoption option;
                switch (params[0] % 5) {
                    case 0: option = CURLMOPT_SOCKETDATA; break;
                    case 1: option = CURLMOPT_TIMERDATA; break;
                    case 2: option = CURLMOPT_PIPELINING; break;
                    case 3: option = CURLMOPT_MAXCONNECTS; break;
                    case 4: option = CURLMOPT_MAX_TOTAL_CONNECTIONS; break;
                    default: option = CURLMOPT_SOCKETDATA; break;
                }
                
                long param = 0;
                if (params_size >= sizeof(long)) {
                    memcpy(&param, params, sizeof(long));
                }
                
                // Call curl_multi_setopt with appropriate parameter types
                switch (option) {
                    case CURLMOPT_SOCKETDATA:
                    case CURLMOPT_TIMERDATA:
                        curl_multi_setopt(multi_handle, option, sockp);
                        break;
                    case CURLMOPT_PIPELINING:
                    case CURLMOPT_MAXCONNECTS:
                    case CURLMOPT_MAX_TOTAL_CONNECTIONS:
                        curl_multi_setopt(multi_handle, option, param);
                        break;
                    default:
                        break;
                }
                break;
            }
            
            case 1: {
                // Test curl_multi_socket (deprecated but still testable)
                curl_multi_socket(multi_handle, sockfd, &running_handles);
                break;
            }
            
            case 2: {
                // Test curl_multi_assign
                curl_multi_assign(multi_handle, sockfd, sockp);
                break;
            }
            
            case 3: {
                // Test curl_multi_socket_all (deprecated but still testable)
                curl_multi_socket_all(multi_handle, &running_handles);
                break;
            }
            
            case 4: {
                // Test curl_multi_perform
                curl_multi_perform(multi_handle, &running_handles);
                break;
            }
            
            case 5: {
                // Test curl_multi_socket_action
                int ev_bitmask = 0;
                if (params_size >= sizeof(int)) {
                    memcpy(&ev_bitmask, params, sizeof(int));
                }
                // Limit ev_bitmask to valid values (CURL_CSELECT_IN, OUT, ERR)
                ev_bitmask = ev_bitmask & (CURL_CSELECT_IN | CURL_CSELECT_OUT | CURL_CSELECT_ERR);
                curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
                break;
            }
        }
    }

    // Clean up
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}