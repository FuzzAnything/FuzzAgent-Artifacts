// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static int dummy_running_handles = 0;
static int dummy_sockp_data = 0;

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzing data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Use fuzzing data to determine which functions to call
    uint8_t selector = Data[0] % 6;
    curl_socket_t dummy_socket = (curl_socket_t)(Data[0] + 1);
    int ev_bitmask = (int)(Data[0] % 8);
    
    switch (selector) {
        case 0:
            curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &dummy_running_handles);
            break;
        case 1:
            curl_multi_socket_all(multi_handle, &dummy_running_handles);
            break;
        case 2:
            curl_multi_assign(multi_handle, dummy_socket, (void *)&dummy_sockp_data);
            break;
        case 3:
            if (Data[0] % 2 == 0) {
                curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
            } else {
                curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
            }
            break;
        case 4:
            curl_multi_perform(multi_handle, &dummy_running_handles);
            break;
        case 5:
            curl_multi_socket(multi_handle, dummy_socket, &dummy_running_handles);
            break;
    }
    
    // Clean up
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}