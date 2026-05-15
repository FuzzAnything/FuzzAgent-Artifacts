// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "curl/curl.h"
#include "curl/multi.h"

#ifdef __cplusplus
}
#endif

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file for potential file:// URLs
    write_dummy_file(Data, Size);
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    // Create and add an easy handle with file:// URL
    CURL *easy = curl_easy_init();
    if (easy) {
        // Use file:// URL with our dummy file
        curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        
        // Add to multi handle
        curl_multi_add_handle(multi_handle, easy);
    }
    
    // Test curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = -1;
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Test curl_multi_timeout
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);
    
    // Test curl_multi_wait with some extra fds based on fuzz data
    int ret = 0;
    unsigned int extra_nfds = Size > 2 ? (Data[0] % 4) : 0;
    struct curl_waitfd *extra_fds = NULL;
    
    if (extra_nfds > 0 && Size >= 1 + extra_nfds * 2) {
        extra_fds = (struct curl_waitfd*)malloc(extra_nfds * sizeof(struct curl_waitfd));
        if (extra_fds) {
            for (unsigned int i = 0; i < extra_nfds; i++) {
                // Use fuzz data to set fd and events
                extra_fds[i].fd = Data[1 + i*2] % 1024;
                extra_fds[i].events = Data[2 + i*2] % 4;
                extra_fds[i].revents = 0;
            }
        }
    }
    
    int timeout = Size > 1 ? (Data[1] % 1000) : 100;
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout, &ret);
    
    free(extra_fds);
    
    // Test curl_multi_waitfds
    unsigned int fd_count = 0;
    // First call to get count
    curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    if (fd_count > 0) {
        struct curl_waitfd *ufds = (struct curl_waitfd*)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            // Second call to get actual fds
            curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }
    
    // Test curl_multi_perform
    int running_handles = 0;
    for (int i = 0; i < 3; i++) { // Perform a few iterations
        curl_multi_perform(multi_handle, &running_handles);
        if (running_handles == 0) break;
    }
    
    // Test deprecated curl_multi_socket with dummy socket
    if (Size > 0) {
        curl_socket_t dummy_socket = Data[0] % 1024;
        curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    }
    
    // Cleanup
    if (easy) {
        curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}