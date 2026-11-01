// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <unistd.h>
#include <sys/select.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize global curl environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle for testing remove_handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle to make it valid for some operations
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzzing logic based on input data
    unsigned int op = Data[0] % 7;

    switch (op) {
        case 0: {
            // Test curl_multi_strerror with various error codes
            // Use input data to generate error codes, ensuring Size > 1
            CURLMcode error_code = (CURLMcode)(Data[1] % 256);
            const char *str = curl_multi_strerror(error_code);
            if (str) {
                // Consume the string to avoid unused variable warnings
                (void)strlen(str);
            }
            break;
        }
        case 1: {
            // Test curl_multi_wakeup
            // This might fail if not in a polling state, but that's okay for fuzzing
            curl_multi_wakeup(multi_handle);
            break;
        }
        case 2: {
            // Test curl_multi_assign
            // Create a dummy socket and pointer
            curl_socket_t sockfd = -1;
            if (Size > 2) {
                sockfd = (curl_socket_t)(Data[1] % 1000);
            }
            void *sockp = (void *)(uintptr_t)(Size > 3 ? Data[2] : 0);
            curl_multi_assign(multi_handle, sockfd, sockp);
            break;
        }
        case 3: {
            // Test curl_multi_init (already done, but we can try again)
            CURLM *new_multi = curl_multi_init();
            if (new_multi) {
                curl_multi_cleanup(new_multi);
            }
            break;
        }
        case 4: {
            // Test curl_multi_remove_handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            break;
        }
        case 5: {
            // Test curl_multi_fdset
            fd_set read_fd_set, write_fd_set, exc_fd_set;
            int max_fd = -1;
            FD_ZERO(&read_fd_set);
            FD_ZERO(&write_fd_set);
            FD_ZERO(&exc_fd_set);
            curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
            break;
        }
        case 6: {
            // Test with invalid handle (NULL)
            curl_multi_wakeup(NULL);
            curl_multi_assign(NULL, -1, NULL);
            curl_multi_remove_handle(NULL, easy_handle);
            break;
        }
        default:
            break;
    }

    // Cleanup
    // Ensure easy handle is removed before cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}