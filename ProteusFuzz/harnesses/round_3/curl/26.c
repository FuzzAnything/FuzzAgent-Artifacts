// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

/* 
 * The error indicates a conflict with typecheck macros in curl/multi.h.
 * To resolve this, we define CURL_DISABLE_TYPECHECK before including curl headers.
 * This disables the GCC-specific extension checks that cause the compilation failure.
 */
#define CURL_DISABLE_TYPECHECK 1

#include <curl/multi.h>

static void setup_dummy_file(void) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        const char *data = "dummy content for fuzzing";
        write(fd, data, strlen(data));
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Initialize easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Prepare extra_fds for polling functions */
    struct curl_waitfd extra_fds[2];
    int extra_nfds = 0;
    int ret = 0;
    int timeout_ms = 0;

    /* Parse input to diversify states */
    if (Size > 0) {
        timeout_ms = (int)Data[0];
        if (Size > 1) {
            extra_nfds = Data[1] % 2; /* Limit to 0 or 1 for safety */
        }
    }

    if (extra_nfds > 0) {
        /* Ensure we have enough data before accessing indices 2 and 3 */
        if (Size >= 3) {
            extra_fds[0].fd = (curl_socket_t)(Data[2] & 0x7F); /* Ensure non-negative */
        } else {
            extra_fds[0].fd = -1;
        }
        
        if (Size >= 4) {
            extra_fds[0].events = (short)(Data[3] % 4);
        } else {
            extra_fds[0].events = 0;
        }
        extra_fds[0].revents = 0;
    }

    /* Test curl_multi_poll */
    curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    /* Test curl_multi_wait */
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    /* Test curl_multi_assign */
    curl_socket_t dummy_socket = -1;
    void *sockp = (void *)Data;
    if (Size > 4) {
        dummy_socket = (curl_socket_t)(Data[4] & 0x7F);
    }
    curl_multi_assign(multi_handle, dummy_socket, sockp);

    /* Test curl_multi_remove_handle */
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Test curl_multi_strerror with various error codes */
    CURLMcode error_codes[] = {
        CURLM_OK, CURLM_CALL_MULTI_PERFORM, CURLM_BAD_HANDLE,
        CURLM_BAD_EASY_HANDLE, CURLM_OUT_OF_MEMORY, CURLM_INTERNAL_ERROR,
        CURLM_BAD_SOCKET, CURLM_UNKNOWN_OPTION, CURLM_ADDED_ALREADY
    };
    for (int i = 0; i < (int)(sizeof(error_codes) / sizeof(CURLMcode)); i++) {
        const char *msg = curl_multi_strerror(error_codes[i]);
        (void)msg; /* Use to avoid unused warning */
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}