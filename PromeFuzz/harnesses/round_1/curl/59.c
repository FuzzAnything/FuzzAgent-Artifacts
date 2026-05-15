// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Disable type checking macros that cause compilation issues
#define CURL_DISABLE_TYPECHECK

// Include curl headers with proper path
#include "curl/multi.h"

// Define a dummy socket callback function
static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

// Define a dummy timer callback function
static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Create a dummy file with fuzzer data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Use fuzzer data to determine which operations to perform
    if (Size > 0) {
        uint8_t action = Data[0];
        int running_handles = 0;
        curl_socket_t dummy_socket = -1;
        
        // Try to get a valid file descriptor for socket operations
        fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            dummy_socket = fd;
        }
        
        // Test different functions based on fuzzer input
        switch (action % 7) {
            case 0:
                // Test curl_multi_setopt with various options
                if (Size > 1) {
                    uint8_t option_selector = Data[1] % 6;
                    switch (option_selector) {
                        case 0:
                            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                            break;
                        case 1:
                            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                            break;
                        case 2:
                            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Size > 2 ? Data[2] : 1));
                            break;
                        case 3:
                            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Size > 2 ? Data[2] : 10));
                            break;
                        case 4:
                            curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)(Size > 2 ? Data[2] : 100));
                            break;
                        case 5:
                            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, (void*)(uintptr_t)(Size > 2 ? Data[2] : 0));
                            break;
                    }
                }
                break;
                
            case 1:
                // Test curl_multi_socket (deprecated)
                if (dummy_socket >= 0) {
                    curl_multi_socket(multi_handle, dummy_socket, &running_handles);
                }
                break;
                
            case 2:
                // Test curl_multi_assign
                if (dummy_socket >= 0) {
                    void *sockp = (void *)(uintptr_t)(Size > 1 ? Data[1] : 0);
                    curl_multi_assign(multi_handle, dummy_socket, sockp);
                }
                break;
                
            case 3:
                // Test curl_multi_socket_all (deprecated)
                curl_multi_socket_all(multi_handle, &running_handles);
                break;
                
            case 4:
                // Test curl_multi_perform
                curl_multi_perform(multi_handle, &running_handles);
                break;
                
            case 5:
                // Test curl_multi_socket_action
                if (dummy_socket >= 0 && Size > 2) {
                    int ev_bitmask = Data[1] % 8;  // Use lower bits for event mask
                    curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
                }
                break;
                
            case 6:
                // Test sequence of operations
                if (Size > 3) {
                    // Set an option
                    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Data[1] % 2));
                    
                    // Perform
                    curl_multi_perform(multi_handle, &running_handles);
                    
                    // Socket action if we have a valid socket
                    if (dummy_socket >= 0) {
                        curl_multi_socket_action(multi_handle, dummy_socket, Data[2] % 8, &running_handles);
                    }
                }
                break;
        }
        
        // Clean up socket if we opened it
        if (dummy_socket >= 0) {
            close(dummy_socket);
        }
    }
    
    // Clean up
    curl_multi_cleanup(multi_handle);
    unlink("./dummy_file");
    
    return 0;
}