// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Use fuzzer data to influence parameters
    int timeout_ms;
    long milliseconds;
    int running_handles;
    int ret;
    
    // For socket-based functions
    curl_socket_t dummy_socket = -1;
    
    // Create a dummy file descriptor for testing
    int fd = open("./dummy_file", O_RDWR | O_CREAT, 0644);
    if (fd >= 0) {
        // Write fuzzer data to file
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
            lseek(fd, 0, SEEK_SET);
        }
        dummy_socket = fd;
    }

    // 1. Test curl_multi_timeout
    curl_multi_timeout(multi_handle, &milliseconds);

    // 2. Test curl_multi_wait with various parameters
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    
    if (Size > sizeof(struct curl_waitfd)) {
        // Use fuzzer data to populate waitfd structures
        memcpy(&extra_fds[0], Data, sizeof(struct curl_waitfd));
        extra_fds[0].fd = dummy_socket;
        extra_fds[0].events = (short)(Data[0] & 0x07); // Valid event bits
        extra_nfds = 1;
        
        // Set timeout based on fuzzer input
        timeout_ms = (int)(Data[0] % 1000);
        curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    }

    // 3. Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // 4. Test curl_multi_poll
    if (Size > sizeof(struct curl_waitfd) * 2) {
        memcpy(&extra_fds[1], Data + sizeof(struct curl_waitfd), sizeof(struct curl_waitfd));
        extra_fds[1].fd = dummy_socket;
        extra_fds[1].events = (short)(Data[1] & 0x07);
        extra_nfds = 2;
        
        timeout_ms = (int)(Data[1] % 1000);
        curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    }

    // 5. Test curl_multi_socket_action with various event masks
    if (dummy_socket != -1) {
        // Test with different event bitmasks from fuzzer input
        int ev_bitmask = (int)(Data[0] & 0x07); // Valid event bits
        curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
        
        // Test with no event
        curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);
    }

    // 6. Test curl_multi_assign
    if (dummy_socket != -1) {
        void *sockp = (void *)(uintptr_t)Data[0];
        curl_multi_assign(multi_handle, dummy_socket, sockp);
        
        // Test with invalid socket
        curl_multi_assign(multi_handle, CURL_SOCKET_BAD, NULL);
    }

    // Cleanup
    if (fd >= 0) {
        close(fd);
        unlink("./dummy_file");
    }
    
    curl_multi_cleanup(multi_handle);
    
    return 0;
}