// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/multi.h"

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Initialize curl globally if needed
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create a dummy easy handle and add it to multi handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Configure with dummy URL
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    // Use first byte to determine which function to call
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0] % 6;
        Data++;
        Size--;
    }

    // Prepare common variables
    int running_handles = 0;
    CURLMcode res;
    curl_socket_t dummy_socket = -1;

    // Create a dummy file descriptor if needed
    const char *dummy_filename = "./dummy_file";
    int fd = open(dummy_filename, O_RDWR | O_CREAT, 0644);
    if (fd >= 0) {
        // Write some data to the file
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
            lseek(fd, 0, SEEK_SET);
        }
        dummy_socket = fd;
    }

    switch (selector) {
        case 0:
            // Test curl_multi_socket_all (deprecated)
            res = curl_multi_socket_all(multi_handle, &running_handles);
            break;
            
        case 1:
            // Test curl_multi_assign
            if (dummy_socket >= 0) {
                void *sockp = (void *)(uintptr_t)Size;
                res = curl_multi_assign(multi_handle, dummy_socket, sockp);
            }
            break;
            
        case 2:
            // Test curl_multi_wait
            {
                struct curl_waitfd extra_fds[1];
                int ret;
                if (dummy_socket >= 0) {
                    extra_fds[0].fd = dummy_socket;
                    extra_fds[0].events = CURL_WAIT_POLLIN;
                    extra_fds[0].revents = 0;
                    int timeout_ms = (Size > 0) ? (Data[0] % 1000) : 100;
                    res = curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
                } else {
                    res = curl_multi_wait(multi_handle, NULL, 0, 100, &ret);
                }
            }
            break;
            
        case 3:
            // Test curl_multi_socket (deprecated)
            if (dummy_socket >= 0) {
                res = curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            }
            break;
            
        case 4:
            // Test curl_multi_socket_action
            if (dummy_socket >= 0) {
                int ev_bitmask = 0;
                if (Size > 0) {
                    ev_bitmask = Data[0] % 4; // 0-3 for different event masks
                }
                res = curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            }
            break;
            
        case 5:
            // Test curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            break;
            
        default:
            res = CURLM_OK;
            break;
    }

    // Clean up
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    if (fd >= 0) {
        close(fd);
        unlink(dummy_filename);
    }
    
    curl_multi_cleanup(multi_handle);
    
    return 0;
}