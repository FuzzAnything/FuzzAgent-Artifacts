// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_callback(void) {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    // Create dummy file with fuzzer data
    int fd = open("./dummy_file", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set some dummy options
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        
        // Add to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // Use first byte to determine which function to test
    uint8_t selector = Size > 0 ? Data[0] : 0;
    
    // Extract parameters from fuzzer data
    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    int ev_bitmask = 0;
    
    if (Size > 1) dummy_socket = (curl_socket_t)(Data[1] + 1);
    if (Size > 2) ev_bitmask = Data[2];
    
    // Test different API functions based on selector
    switch (selector % 6) {
        case 0:
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 1:
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        case 2:
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            break;
        case 3:
            curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            break;
        case 4:
            // Test curl_multi_setopt with various options
            if (Size > 3) {
                int option_selector = Data[3] % 5;
                switch (option_selector) {
                    case 0:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
                        break;
                    case 1:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, NULL);
                        break;
                    case 2:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
                        break;
                    case 3:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, NULL);
                        break;
                    case 4:
                        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)Data[4]);
                        break;
                }
            }
            break;
        case 5:
            curl_multi_assign(multi_handle, dummy_socket, (void*)(uintptr_t)Size);
            break;
    }
    
    // Clean up
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}

#ifdef __cplusplus
}
#endif