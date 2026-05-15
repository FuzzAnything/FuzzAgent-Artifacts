// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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

static void dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {}
static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) { return 0; }

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential use
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

    // Create dummy easy handle with dummy URL
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_NOSIGNAL, 1L);
        curl_multi_add_handle(multi_handle, easy);
    }

    // Extract parameters from fuzzer input
    int running_handles = 0;
    int ret_val = 0;
    curl_socket_t sock = (curl_socket_t)(Size > 0 ? Data[0] : 0);
    int ev_bitmask = Size > 1 ? Data[1] : 0;
    int timeout_ms = Size > 2 ? Data[2] : 0;
    void *sockp = Size > 3 ? (void *)(uintptr_t)Data[3] : NULL;

    // Prepare waitfd structures
    struct curl_waitfd extra_fds[2];
    if (Size > 4) {
        extra_fds[0].fd = (curl_socket_t)Data[4];
        extra_fds[0].events = Size > 5 ? Data[5] : 0;
        extra_fds[0].revents = 0;
        extra_fds[1].fd = (curl_socket_t)(Size > 6 ? Data[6] : 0);
        extra_fds[1].events = Size > 7 ? Data[7] : 0;
        extra_fds[1].revents = 0;
    }

    // Call target functions with fuzzer-derived parameters
    curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    curl_multi_assign(multi_handle, sock, sockp);
    curl_multi_perform(multi_handle, &running_handles);
    curl_multi_wait(multi_handle, extra_fds, Size > 4 ? 2 : 0, timeout_ms, &ret_val);
    
    // Call deprecated functions for coverage
    curl_multi_socket(multi_handle, sock, &running_handles);
    curl_multi_socket_all(multi_handle, &running_handles);

    // Cleanup
    if (easy) {
        curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi_handle);

    // Clean up dummy file
    unlink("./dummy_file");

    return 0;
}