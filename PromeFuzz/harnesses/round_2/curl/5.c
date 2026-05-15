// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_NO_OLDIES
#include "curl/curl.h"
#include "curl/multi.h"

static void dummy_callback(void) {
    // Empty callback for socket actions
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Configure easy handle to read from dummy file
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, NULL);
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        int running_handles = 0;
        
        // Use input bytes to drive different function calls
        for (size_t i = 0; i < Size && i < 256; i++) {
            uint8_t action = Data[i] % 6;
            
            switch (action) {
                case 0:
                    // curl_multi_socket_action with timeout socket
                    curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);
                    break;
                    
                case 1:
                    // curl_multi_waitfds
                    {
                        unsigned int fd_count = 0;
                        // First call to get count
                        curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
                        if (fd_count > 0) {
                            struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                            if (ufds) {
                                // Second call to get actual fds
                                curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
                                free(ufds);
                            }
                        }
                    }
                    break;
                    
                case 2:
                    // curl_multi_remove_handle and re-add
                    curl_multi_remove_handle(multi_handle, easy_handle);
                    curl_multi_add_handle(multi_handle, easy_handle);
                    break;
                    
                case 3:
                    // curl_multi_add_handle (already added, but call anyway)
                    curl_multi_add_handle(multi_handle, easy_handle);
                    break;
                    
                case 4:
                    // curl_multi_wait with varying timeouts
                    {
                        int timeout_ms = (Data[i] % 100) + 1;  // 1-100 ms
                        int ret = 0;
                        curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret);
                    }
                    break;
                    
                case 5:
                    // curl_multi_perform
                    curl_multi_perform(multi_handle, &running_handles);
                    break;
            }
            
            // Occasionally call socket action with different event masks
            if (i % 4 == 0) {
                int ev_bitmask = Data[i] % 8;  // 0-7 for different event combinations
                curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, ev_bitmask, &running_handles);
            }
        }
        
        // Final cleanup
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}