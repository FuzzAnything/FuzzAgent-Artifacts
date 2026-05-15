// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

#include <unistd.h>
#include <fcntl.h>

static size_t dummy_read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    return 0;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    int ret_val = 0;
    unsigned int fd_count = 0;
    struct curl_waitfd *ufds = NULL;
    struct curl_waitfd extra_fds[2];
    int timeout_ms = 0;
    curl_socket_t dummy_socket = -1;
    
    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    // Initialize easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    // Configure easy handle with dummy options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);
    
    // Create dummy file with fuzzer data
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Use fuzzer data to influence parameters
    if (Size >= 1) {
        // Use first byte to determine which functions to call
        uint8_t selector = Data[0];
        
        // Test curl_multi_waitfds
        if (selector & 0x01) {
            // First call to get count
            curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
            
            if (fd_count > 0) {
                ufds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    // Second call with allocated buffer
                    curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
                    free(ufds);
                }
            }
        }
        
        // Test curl_multi_wait
        if (Size >= 2) {
            timeout_ms = Data[1] % 1000;  // Limit timeout to 1000ms
            
            // Set up some dummy file descriptors
            extra_fds[0].fd = 0;  // stdin
            extra_fds[0].events = CURL_WAIT_POLLIN;
            extra_fds[0].revents = 0;
            
            extra_fds[1].fd = 1;  // stdout
            extra_fds[1].events = CURL_WAIT_POLLOUT;
            extra_fds[1].revents = 0;
            
            unsigned int extra_nfds = (Size >= 3) ? (Data[2] % 3) : 1;
            curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
        }
        
        // Test curl_multi_perform
        if (selector & 0x02) {
            curl_multi_perform(multi_handle, &running_handles);
        }
        
        // Test curl_multi_socket (deprecated but still testable)
        if (selector & 0x04) {
            dummy_socket = (Size >= 4) ? (curl_socket_t)(Data[3]) : 0;
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
        }
        
        // Test curl_multi_remove_handle and re-add
        if (selector & 0x08) {
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_add_handle(multi_handle, easy_handle);
        }
        
        // Test curl_multi_wakeup
        if (selector & 0x10) {
            curl_multi_wakeup(multi_handle);
        }
    }
    
    // Perform one more multi_perform to ensure state exploration
    curl_multi_perform(multi_handle, &running_handles);
    
    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}