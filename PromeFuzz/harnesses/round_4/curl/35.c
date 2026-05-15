// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
#include "curl/multi.h"
#include "curl/easy.h"

static void dummy_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    static char dummy_file[] = "./dummy_file";
    int fd = open(dummy_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (fd >= 0) unlink(dummy_file);
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    int running_handles = 0;
    int ret_val = 0;
    curl_socket_t dummy_sock = 0;
    void *dummy_ptr = NULL;
    struct curl_waitfd extra_fds[2];
    int timeout_ms = 0;
    int ev_bitmask = 0;

    if (Size > 0) {
        uint8_t selector = Data[0];
        size_t offset = 1;

        if (selector & 0x01) {
            curl_multi_socket_all(multi_handle, &running_handles);
        }

        if (selector & 0x02 && Size > offset) {
            dummy_sock = (curl_socket_t)Data[offset];
            offset = (offset + 1) % Size;
            curl_multi_assign(multi_handle, dummy_sock, dummy_ptr);
        }

        if (selector & 0x04 && Size > offset + 3) {
            extra_fds[0].fd = (curl_socket_t)Data[offset];
            extra_fds[0].events = (short)Data[offset + 1];
            extra_fds[0].revents = 0;
            extra_fds[1].fd = (curl_socket_t)Data[offset + 2];
            extra_fds[1].events = (short)Data[offset + 3];
            extra_fds[1].revents = 0;
            timeout_ms = (int)(Data[offset % Size] * 10);
            curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret_val);
        }

        if (selector & 0x08 && Size > offset) {
            dummy_sock = (curl_socket_t)Data[offset];
            offset = (offset + 1) % Size;
            curl_multi_socket(multi_handle, dummy_sock, &running_handles);
        }

        if (selector & 0x10 && Size > offset + 1) {
            dummy_sock = (curl_socket_t)Data[offset];
            ev_bitmask = (int)Data[offset + 1];
            curl_multi_socket_action(multi_handle, dummy_sock, ev_bitmask, &running_handles);
        }

        if (selector & 0x20) {
            curl_multi_perform(multi_handle, &running_handles);
        }
    }

    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    if (fd >= 0) unlink(dummy_file);
    return 0;
}