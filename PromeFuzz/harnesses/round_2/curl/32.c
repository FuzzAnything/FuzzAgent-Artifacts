// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/multi.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_socket_callback(CURL *easy, curl_socket_t s, int what,
                                 void *userp, void *socketp) {}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

static void *dummy_sockp = NULL;

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential file-based transfers
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    // Create and add a dummy easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // Use fuzzer data to determine which function to call
    // and derive parameters from the input
    if (Size > 0) {
        uint8_t selector = Data[0] % 6;
        int running_handles = 0;
        curl_socket_t dummy_socket = 0; // Using stdin as dummy socket
        
        switch (selector) {
            case 0: {
                // curl_multi_socket_action
                int ev_bitmask = (Size > 1) ? (Data[1] % 4) : 0;
                curl_multi_socket_action(multi_handle, dummy_socket, 
                                        ev_bitmask, &running_handles);
                break;
            }
            case 1: {
                // curl_multi_socket_all (deprecated)
                #ifdef CURL_DEPRECATED
                curl_multi_socket_all(multi_handle, &running_handles);
                #endif
                break;
            }
            case 2: {
                // curl_multi_setopt
                // Try different options based on input
                if (Size > 1) {
                    uint8_t opt_selector = Data[1] % 3;
                    switch (opt_selector) {
                        case 0:
                            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, 
                                            dummy_socket_callback);
                            break;
                        case 1:
                            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, 
                                            dummy_timer_callback);
                            break;
                        case 2:
                            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 
                                            (long)((Size > 2) ? Data[2] : 10));
                            break;
                    }
                }
                break;
            }
            case 3: {
                // curl_multi_assign
                curl_multi_assign(multi_handle, dummy_socket, dummy_sockp);
                break;
            }
            case 4: {
                // curl_multi_socket (deprecated)
                #ifdef CURL_DEPRECATED
                curl_multi_socket(multi_handle, dummy_socket, &running_handles);
                #endif
                break;
            }
            case 5: {
                // curl_multi_perform
                curl_multi_perform(multi_handle, &running_handles);
                break;
            }
        }
        
        // Try calling curl_multi_perform again to explore more states
        if (Size > 2 && (Data[2] % 2)) {
            curl_multi_perform(multi_handle, &running_handles);
        }
    }
    
    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}