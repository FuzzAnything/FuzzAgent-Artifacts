// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for potential file operations
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
    
    // Parse fuzzer data for parameters
    // Use first byte for socket descriptor (modulo to keep it reasonable)
    curl_socket_t sock = (Data && Size > 0) ? (Data[0] % 1024) : 0;
    
    // Use second byte for event bitmask (mask to valid bits: 0-7)
    int ev_bitmask = (Size > 1) ? (Data[1] & 0x07) : 0;
    
    // Use third byte for option selection
    int option_selector = (Size > 2) ? Data[2] : 0;
    
    // Use fourth byte for running_handles value
    int running_handles = (Size > 3) ? Data[3] : 0;
    
    // Use remaining data for sockp pointer data if needed
    void *sockp = (Size > 4) ? (void *)(Data + 4) : NULL;
    
    // Test curl_multi_socket_action
    int running1 = 0;
    curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running1);
    
    // Test curl_multi_assign
    curl_multi_assign(multi_handle, sock, sockp);
    
    // Test curl_multi_setopt with various options
    // Use if-else instead of switch to avoid macro expansion issues
    if (option_selector % 6 == 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, sockp);
    } else if (option_selector % 6 == 1) {
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, sockp);
    } else if (option_selector % 6 == 2) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)running_handles);
    } else if (option_selector % 6 == 3) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)running_handles);
    } else if (option_selector % 6 == 4) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)running_handles);
    } else if (option_selector % 6 == 5) {
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)running_handles);
    }
    
    // Test curl_multi_perform
    int running2 = 0;
    curl_multi_perform(multi_handle, &running2);
    
    // Test deprecated curl_multi_socket (still callable for coverage)
    int running3 = 0;
    curl_multi_socket(multi_handle, sock, &running3);
    
    // Test deprecated curl_multi_socket_all
    int running4 = 0;
    curl_multi_socket_all(multi_handle, &running4);
    
    // Clean up
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}