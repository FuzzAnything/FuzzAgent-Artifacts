// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Define CURL_DISABLE_TYPECHECK to avoid macro issues with curl_multi_setopt */
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static int socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential use
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
    
    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Use fuzzer data to determine which operations to perform
    if (Size > 0) {
        uint8_t operation = Data[0] % 7;
        size_t data_offset = 1;
        
        switch (operation) {
            case 0: {
                // Test curl_multi_socket_action
                curl_socket_t socket = (curl_socket_t)(Size > 1 ? Data[1] : 0);
                int ev_bitmask = (int)(Size > 2 ? Data[2] : 0);
                int running_handles;
                curl_multi_socket_action(multi_handle, socket, ev_bitmask, &running_handles);
                break;
            }
            case 1: {
                // Test curl_multi_setopt with various options
                if (Size > data_offset) {
                    uint8_t option_index = Data[data_offset] % 10;
                    data_offset++;
                    
                    switch (option_index) {
                        case 0:
                            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, socket_callback);
                            break;
                        case 1:
                            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, timer_callback);
                            break;
                        case 2:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, value);
                            }
                            break;
                        case 3:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, value);
                            }
                            break;
                        case 4:
                            if (Size > data_offset) {
                                long value = (long)(Data[data_offset] % 3);
                                curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, value);
                            }
                            break;
                        case 5:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, value);
                            }
                            break;
                        case 6:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, value);
                            }
                            break;
                        case 7:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, value);
                            }
                            break;
                        case 8:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, value);
                            }
                            break;
                        case 9:
                            if (Size > data_offset) {
                                long value = (long)Data[data_offset];
                                curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING_SITE_BL, value);
                            }
                            break;
                    }
                }
                break;
            }
            case 2: {
                // Test curl_multi_remove_handle
                curl_multi_remove_handle(multi_handle, easy_handle);
                // Re-add for subsequent operations
                curl_multi_add_handle(multi_handle, easy_handle);
                break;
            }
            case 3: {
                // Test curl_multi_perform
                int running_handles;
                curl_multi_perform(multi_handle, &running_handles);
                break;
            }
            case 4: {
                // Test deprecated curl_multi_socket
                curl_socket_t socket = (curl_socket_t)(Size > 1 ? Data[1] : 0);
                int running_handles;
                curl_multi_socket(multi_handle, socket, &running_handles);
                break;
            }
            case 5: {
                // Test curl_multi_wakeup
                curl_multi_wakeup(multi_handle);
                break;
            }
            case 6: {
                // Test multiple operations in sequence
                int running_handles;
                curl_multi_perform(multi_handle, &running_handles);
                
                if (Size > 1) {
                    curl_socket_t socket = (curl_socket_t)Data[1];
                    int ev_bitmask = (int)(Size > 2 ? Data[2] : 0);
                    curl_multi_socket_action(multi_handle, socket, ev_bitmask, &running_handles);
                }
                
                curl_multi_wakeup(multi_handle);
                break;
            }
        }
    }
    
    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}