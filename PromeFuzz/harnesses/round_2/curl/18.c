// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "/promefuzz/database/curl/latest/code/include/curl/multi.h"
#include "/promefuzz/database/curl/latest/code/include/curl/easy.h"

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Initialize dummy file
    int fd = open("./dummy_file", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    // Initialize curl
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_setopt with various options
    if (Size > 0) {
        // Test different integer options
        long int_val = (Size > 1) ? (long)Data[1] : 1L;
        
        // Test various options
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, int_val);
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, int_val);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, int_val);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, int_val);
    }

    // Fuzz curl_multi_socket_action
    if (Size > 2) {
        curl_socket_t sock = (curl_socket_t)(Data[1] % 256);
        int ev_bitmask = Data[2] % 8;
        int running_handles = 0;
        
        CURLMcode action_result = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
        const char *error_str = curl_multi_strerror(action_result);
        (void)error_str;
    }

    // Fuzz curl_multi_wakeup
    if (Size > 3) {
        CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
        const char *wakeup_error = curl_multi_strerror(wakeup_result);
        (void)wakeup_error;
    }

    // Fuzz deprecated curl_multi_socket (for coverage)
    if (Size > 4) {
        curl_socket_t sock2 = (curl_socket_t)(Data[3] % 256);
        int running_handles2 = 0;
        
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        CURLMcode socket_result = curl_multi_socket(multi_handle, sock2, &running_handles2);
        #pragma GCC diagnostic pop
        
        const char *socket_error = curl_multi_strerror(socket_result);
        (void)socket_error;
    }

    // Fuzz curl_multi_remove_handle
    if (Size > 5) {
        CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
        const char *remove_error = curl_multi_strerror(remove_result);
        (void)remove_error;
        
        // Re-add handle for cleanup
        if (remove_result == CURLM_OK) {
            curl_multi_add_handle(multi_handle, easy_handle);
        }
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}