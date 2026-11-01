// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static int create_dummy_socket(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    int val = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Use a dummy file or simple URL
    if (Size > 0 && Data[0] & 1) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:1");
    } else {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    }
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);

    CURLMcode mcode;
    mcode = curl_multi_add_handle(multi_handle, easy_handle);
    if (mcode != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_BAD;
    int ev_bitmask = 0;

    // Fuzz curl_multi_socket_action
    if (Size > 1) {
        s = (curl_socket_t)(Data[0] & 0xFF);
        ev_bitmask = (int)(Data[1] & 0xFF);
        // Try with a real socket if possible
        if (s == 0) {
            s = create_dummy_socket();
        }
        if (s != CURL_SOCKET_BAD) {
            curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
            if (s > 0) {
                close(s);
            }
        }
    }

    // Fuzz curl_multi_socket (deprecated)
    if (Size > 2) {
        s = (curl_socket_t)(Data[2] & 0xFF);
        if (s == 0) {
            s = create_dummy_socket();
        }
        if (s != CURL_SOCKET_BAD) {
            curl_multi_socket(multi_handle, s, &running_handles);
            if (s > 0) {
                close(s);
            }
        }
    }

    // Fuzz curl_multi_socket_all (deprecated)
    if (Size > 3) {
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    // Fuzz curl_multi_assign
    if (Size > 5) {
        curl_socket_t sockfd = (curl_socket_t)(Data[4] & 0xFF);
        if (sockfd == 0) {
            sockfd = create_dummy_socket();
        }
        void *sockp = (void *)(uintptr_t)(Data[5] & 0xFF);
        if (sockfd != CURL_SOCKET_BAD) {
            curl_multi_assign(multi_handle, sockfd, sockp);
            if (sockfd > 0) {
                close(sockfd);
            }
        }
    }

    // Fuzz curl_multi_perform
    if (Size > 5) {
        curl_multi_perform(multi_handle, &running_handles);
    }

    // Fuzz curl_multi_strerror with various codes
    if (Size > 6) {
        CURLMcode error_code = (CURLMcode)(Data[6] & 0xFF);
        const char *str = curl_multi_strerror(error_code);
        (void)str; // Use the result to avoid warnings
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}