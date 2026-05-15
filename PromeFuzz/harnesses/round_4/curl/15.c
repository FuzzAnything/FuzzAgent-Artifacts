// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "multi.h"
#include "easy.h"

#ifdef __cplusplus
}
#endif

static CURLM *multi_handle = NULL;
static CURL *easy_handle = NULL;

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Initialize multi handle if not already done
    if (!multi_handle) {
        multi_handle = curl_multi_init();
        if (!multi_handle) {
            return 0;
        }
    }
    
    // Initialize easy handle if not already done
    if (!easy_handle) {
        easy_handle = curl_easy_init();
        if (!easy_handle) {
            curl_multi_cleanup(multi_handle);
            multi_handle = NULL;
            return 0;
        }
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Test curl_multi_waitfds
    unsigned int fd_count = 0;
    // First call to get count
    curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    if (fd_count > 0) {
        struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            // Second call with allocated array
            curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }
    
    // Test curl_multi_wakeup
    curl_multi_wakeup(multi_handle);
    
    // Test curl_multi_wait
    int ret = 0;
    int timeout_ms = 10; // Short timeout for fuzzing
    
    // Create some extra fds based on fuzzer input
    unsigned int extra_nfds = Size > 0 ? (Data[0] % 4) : 0;
    struct curl_waitfd extra_fds[4];
    
    for (unsigned int i = 0; i < extra_nfds && i < 4; i++) {
        extra_fds[i].fd = (curl_socket_t)(i + 1);
        extra_fds[i].events = (short)((i < Size - 1) ? (Data[i + 1] % 4) : 0);
        extra_fds[i].revents = 0;
    }
    
    curl_multi_wait(multi_handle, extra_nfds > 0 ? extra_fds : NULL, 
                   extra_nfds, timeout_ms, &ret);
    
    // Test curl_multi_socket (deprecated but still callable)
    int running_handles = 0;
    curl_socket_t dummy_socket = (curl_socket_t)(Size > 0 ? Data[0] : 0);
    curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    
    // Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);
    
    // Test curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up
    curl_easy_cleanup(easy_handle);
    easy_handle = NULL;
    
    curl_multi_cleanup(multi_handle);
    multi_handle = NULL;
    
    return 0;
}