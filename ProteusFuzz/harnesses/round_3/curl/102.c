// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
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
#include <sys/un.h>

static int create_socket_pair(curl_socket_t *sock1, curl_socket_t *sock2) {
    int fds[2];
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    if (ret < 0) {
        *sock1 = CURL_SOCKET_BAD;
        *sock2 = CURL_SOCKET_BAD;
        return -1;
    }
    *sock1 = fds[0];
    *sock2 = fds[1];
    return 0;
}

static void cleanup_socket(curl_socket_t sock) {
    if (sock != CURL_SOCKET_BAD) {
        close(sock);
    }
}

static void cleanup_multi_handle(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sock1 = CURL_SOCKET_BAD;
    curl_socket_t sock2 = CURL_SOCKET_BAD;
    CURLMcode res;
    const char *str;

    // Test curl_multi_strerror with various inputs derived from fuzz data
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLMcode code = (CURLMcode)(Data[i] | (i << 8));
        str = curl_multi_strerror(code);
        if (str) {
            // Consume the string to avoid unused variable warnings
            volatile const char *s = str;
            (void)s;
        }
    }

    // Create sockets for testing socket-related functions
    if (create_socket_pair(&sock1, &sock2) == 0) {
        // Test curl_multi_assign
        res = curl_multi_assign(multi, sock1, (void*)Data);
        (void)res;

        // Test curl_multi_perform
        res = curl_multi_perform(multi, &running_handles);
        (void)res;

        // Test curl_multi_socket (deprecated)
        res = curl_multi_socket(multi, sock1, &running_handles);
        (void)res;

        // Test curl_multi_socket_action
        int ev_bitmask = (Size > 1) ? (Data[1] & 7) : 0;
        res = curl_multi_socket_action(multi, sock1, ev_bitmask, &running_handles);
        (void)res;

        // Test curl_multi_socket_all (deprecated)
        res = curl_multi_socket_all(multi, &running_handles);
        (void)res;

        // Cleanup sockets
        cleanup_socket(sock1);
        cleanup_socket(sock2);
    }

    // Test with invalid socket (CURL_SOCKET_BAD)
    res = curl_multi_socket_action(multi, CURL_SOCKET_BAD, 0, &running_handles);
    (void)res;

    // Cleanup
    cleanup_multi_handle(multi);

    return 0;
}