// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, fp);
        } else {
            const char *default_data = "dummy data";
            fwrite(default_data, 1, strlen(default_data), fp);
        }
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_33(const uint8_t *data, size_t size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    int ret_val = 0;
    unsigned int fd_count = 0;
    struct curl_waitfd *ufds = NULL;
    struct curl_waitfd extra_fds[2];
    int timeout_ms = 0;
    
    // Create dummy file for potential file operations
    create_dummy_file(data, size);
    
    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Initialize easy handle with dummy file URL
    easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // Use input data to determine behavior
    if (size > 0) {
        // Use first byte to determine which functions to call
        uint8_t selector = data[0];
        
        // Test curl_multi_waitfds
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
        
        // Test curl_multi_wait
        // Set up some dummy file descriptors
        extra_fds[0].fd = 0;  // stdin
        extra_fds[0].events = (selector & 0x01) ? CURL_WAIT_POLLIN : CURL_WAIT_POLLOUT;
        extra_fds[0].revents = 0;
        
        extra_fds[1].fd = 1;  // stdout
        extra_fds[1].events = (selector & 0x02) ? CURL_WAIT_POLLIN : CURL_WAIT_POLLOUT;
        extra_fds[1].revents = 0;
        
        // Use second byte for timeout if available
        timeout_ms = (size > 1) ? (int)(data[1] * 10) : 100;
        
        curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret_val);
        
        // Test curl_multi_perform
        curl_multi_perform(multi_handle, &running_handles);
        
        // Test curl_multi_socket (deprecated but still available)
        if (size > 2) {
            curl_socket_t socket_fd = (curl_socket_t)(data[2]);
            curl_multi_socket(multi_handle, socket_fd, &running_handles);
        }
        
        // Test curl_multi_remove_handle if we have an easy handle
        if (easy_handle && (selector & 0x04)) {
            curl_multi_remove_handle(multi_handle, easy_handle);
            // Re-add for cleanup
            if (selector & 0x08) {
                curl_multi_add_handle(multi_handle, easy_handle);
            }
        }
        
        // Test curl_multi_wakeup
        if (selector & 0x10) {
            curl_multi_wakeup(multi_handle);
        }
    } else {
        // Minimal testing with empty input
        curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
        curl_multi_perform(multi_handle, &running_handles);
        curl_multi_wakeup(multi_handle);
    }
    
    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    cleanup_dummy_file();
    
    return 0;
}