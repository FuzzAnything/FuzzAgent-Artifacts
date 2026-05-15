// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    // Use fuzzer data to determine which functions to call
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    
    // Common variables
    int running_handles = 0;
    curl_socket_t sock_fd = 0;
    void *sockp = NULL;
    
    // Extract parameters from fuzzer data if available
    if (data_offset + sizeof(curl_socket_t) <= Size) {
        memcpy(&sock_fd, Data + data_offset, sizeof(curl_socket_t));
        data_offset += sizeof(curl_socket_t);
    }
    
    if (data_offset + sizeof(void*) <= Size) {
        memcpy(&sockp, Data + data_offset, sizeof(void*));
        data_offset += sizeof(void*);
    }
    
    // Call selected function
    switch (selector) {
        case 0: { // curl_multi_setopt
            CURLMoption opt = (CURLMoption)(Data[data_offset % Size] % 256);
            // Try different parameter types based on option
            if (opt % 3 == 0) {
                curl_multi_setopt(multi_handle, opt, (long)(Data[data_offset % Size]));
            } else if (opt % 3 == 1) {
                curl_multi_setopt(multi_handle, opt, dummy_callback);
            } else {
                curl_multi_setopt(multi_handle, opt, sockp);
            }
            break;
        }
        case 1: { // curl_multi_socket (deprecated)
            curl_multi_socket(multi_handle, sock_fd, &running_handles);
            break;
        }
        case 2: { // curl_multi_assign
            curl_multi_assign(multi_handle, sock_fd, sockp);
            break;
        }
        case 3: { // curl_multi_socket_all (deprecated)
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        }
        case 4: { // curl_multi_perform
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
        case 5: { // curl_multi_socket_action
            int ev_bitmask = (data_offset < Size) ? (Data[data_offset] & 0x07) : 0;
            curl_multi_socket_action(multi_handle, sock_fd, ev_bitmask, &running_handles);
            break;
        }
    }
    
    // Clean up
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}