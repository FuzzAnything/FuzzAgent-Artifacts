// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file if needed
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
        }
        close(fd);
    }
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create handles
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (multi_handle && easy_handle) {
        // Set a dummy URL on easy handle
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/dummy");
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Use first byte to determine which function to test
        uint8_t selector = Data[0] % 6;
        int running_handles = 0;
        CURLMcode res;
        
        switch (selector) {
            case 0: {
                // Test curl_multi_socket (deprecated)
                curl_socket_t sock = (curl_socket_t)(Data[1] % 256);
                res = curl_multi_socket(multi_handle, sock, &running_handles);
                const char *err_str = curl_multi_strerror(res);
                (void)err_str; // Use error string to avoid unused variable warning
                break;
            }
            
            case 1: {
                // Test curl_multi_remove_handle
                res = curl_multi_remove_handle(multi_handle, easy_handle);
                const char *err_str = curl_multi_strerror(res);
                (void)err_str;
                // Re-add for subsequent operations
                if (res == CURLM_OK) {
                    curl_multi_add_handle(multi_handle, easy_handle);
                }
                break;
            }
            
            case 2: {
                // Test curl_multi_wakeup
                res = curl_multi_wakeup(multi_handle);
                const char *err_str = curl_multi_strerror(res);
                (void)err_str;
                break;
            }
            
            case 3: {
                // Test curl_multi_socket_action
                curl_socket_t sock = (curl_socket_t)(Data[1] % 256);
                int ev_bitmask = Data[2] % 8; // 3 bits for events
                res = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
                const char *err_str = curl_multi_strerror(res);
                (void)err_str;
                break;
            }
            
            case 4: {
                // Test curl_multi_setopt with various options
                int opt_selector = Data[1] % 5;
                switch (opt_selector) {
                    case 0:
                        res = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                        break;
                    case 1:
                        res = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, NULL);
                        break;
                    case 2:
                        res = curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                        break;
                    case 3:
                        res = curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, NULL);
                        break;
                    case 4:
                        res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Data[2] % 100));
                        break;
                }
                const char *err_str = curl_multi_strerror(res);
                (void)err_str;
                break;
            }
            
            case 5: {
                // Test curl_multi_strerror with various error codes
                int err_selector = Data[1] % 10;
                CURLMcode test_codes[] = {
                    CURLM_OK,
                    CURLM_BAD_HANDLE,
                    CURLM_BAD_EASY_HANDLE,
                    CURLM_OUT_OF_MEMORY,
                    CURLM_INTERNAL_ERROR,
                    CURLM_BAD_SOCKET,
                    CURLM_UNKNOWN_OPTION,
                    CURLM_ADDED_ALREADY,
                    CURLM_RECURSIVE_API_CALL,
                    CURLM_WAKEUP_FAILURE
                };
                if (err_selector < 10) {
                    const char *err_str = curl_multi_strerror(test_codes[err_selector]);
                    (void)err_str;
                }
                break;
            }
        }
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Clean up
    if (easy_handle) curl_easy_cleanup(easy_handle);
    if (multi_handle) curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}