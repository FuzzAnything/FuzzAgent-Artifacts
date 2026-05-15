// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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

static int dummy_fd = -1;

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    if (dummy_fd >= 0) {
        close(dummy_fd);
    }
    dummy_fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dummy_fd >= 0) {
        write(dummy_fd, Data, Size);
        close(dummy_fd);
        dummy_fd = -1;
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    int running_handles = 0;
    int ret_val = 0;
    curl_socket_t sock = (curl_socket_t)(Data[0] | (Data[1] << 8));
    int ev_bitmask = Data[2] & 0x7F;
    int timeout_ms = (Data[3] << 8) | Data[0];
    if (timeout_ms < 0) timeout_ms = 0;
    if (timeout_ms > 1000) timeout_ms = 1000;

    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = sock;
    extra_fds[0].events = (short)(ev_bitmask & 0x3);
    extra_fds[0].revents = 0;
    extra_fds[1].fd = (curl_socket_t)(Data[1] | (Data[2] << 8));
    extra_fds[1].events = (short)((ev_bitmask >> 2) & 0x3);
    extra_fds[1].revents = 0;

    void *sockp = (void *)(uintptr_t)Data[0];

    int func_selector = Size % 6;
    switch (func_selector) {
        case 0:
            curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
            break;
        case 1:
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        case 2:
            curl_multi_assign(multi_handle, sock, sockp);
            break;
        case 3:
            curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret_val);
            break;
        case 4:
            curl_multi_socket(multi_handle, sock, &running_handles);
            break;
        case 5:
            curl_multi_perform(multi_handle, &running_handles);
            break;
    }

    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    curl_multi_cleanup(multi_handle);
    cleanup_dummy_file();

    return 0;
}