// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"
#include <unistd.h>
#include <fcntl.h>

static CURLM* multi_handle = NULL;
static CURL* easy_handle = NULL;
static int dummy_fd = -1;

static void cleanup() {
    if (easy_handle) {
        if (multi_handle) {
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
        curl_easy_cleanup(easy_handle);
        easy_handle = NULL;
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
        multi_handle = NULL;
    }
    if (dummy_fd != -1) {
        close(dummy_fd);
        dummy_fd = -1;
    }
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Initialize multi handle if not already done
    if (!multi_handle) {
        multi_handle = curl_multi_init();
        if (!multi_handle) return 0;
    }

    // Create dummy file with fuzzer data
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Create easy handle and set up a simple transfer
    if (!easy_handle) {
        easy_handle = curl_easy_init();
        if (easy_handle) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
            curl_multi_add_handle(multi_handle, easy_handle);
        }
    }

    // Create dummy socket fd if needed
    if (dummy_fd == -1) {
        dummy_fd = open("./dummy_file", O_RDONLY);
    }

    // Test curl_multi_waitfds
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    if (fd_count > 0) {
        struct curl_waitfd* ufds = malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }

    // Test curl_multi_perform
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // Test curl_multi_wait with various timeouts
    int ret = 0;
    struct curl_waitfd extra_fd = {0};
    if (dummy_fd != -1) {
        extra_fd.fd = dummy_fd;
        extra_fd.events = CURL_WAIT_POLLIN;
    }
    
    // Try different timeout values based on input size
    int timeout = Size > 0 ? (Data[0] % 100) : 10;
    curl_multi_wait(multi_handle, &extra_fd, 1, timeout, &ret);
    
    // Test with no extra fds
    curl_multi_wait(multi_handle, NULL, 0, timeout, &ret);

    // Test curl_multi_socket (deprecated but still available)
    curl_socket_t dummy_socket = 0;
    if (dummy_fd != -1) {
        dummy_socket = dummy_fd;
    }
    curl_multi_socket(multi_handle, dummy_socket, &running_handles);

    // Test curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Test curl_multi_remove_handle and re-add
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    // Clean up periodically to avoid resource exhaustion
    static int call_count = 0;
    call_count++;
    if (call_count % 100 == 0) {
        cleanup();
    }

    return 0;
}