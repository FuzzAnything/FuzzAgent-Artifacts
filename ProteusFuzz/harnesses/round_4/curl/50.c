// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

static int dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userp) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userp;
    return 0;
}

static int dummy_write_callback(char *buffer, size_t size, size_t nitems, void *userp) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userp;
    return size * nitems;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
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

    // Initialize with a dummy URL
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);

    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t sock = -1;
    void *sockp = NULL;
    CURLMcode res;

    // Parse input to decide actions
    uint8_t action = Data[0] % 6;
    int socket_fd = -1;

    // Create a dummy socket for testing curl_multi_assign and curl_multi_socket
    if (Size > 1 && (Data[1] % 2) == 1) {
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd >= 0) {
            // Set non-blocking to avoid hanging
            int flags = fcntl(socket_fd, F_GETFL, 0);
            fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
        }
    }

    switch (action) {
        case 0:
            // Test curl_multi_socket_all
            res = curl_multi_socket_all(multi_handle, &running_handles);
            break;

        case 1:
            // Test curl_multi_assign
            if (socket_fd >= 0) {
                res = curl_multi_assign(multi_handle, socket_fd, (void *)(uintptr_t)(Size > 1 ? Data[1] : 0));
            } else {
                res = curl_multi_assign(multi_handle, -1, NULL);
            }
            break;

        case 2:
            // Test curl_multi_socket
            if (socket_fd >= 0) {
                res = curl_multi_socket(multi_handle, socket_fd, &running_handles);
            } else {
                res = curl_multi_socket(multi_handle, -1, &running_handles);
            }
            break;

        case 3:
            // Test curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            break;

        case 4:
            // Test curl_multi_strerror with various error codes
            // Ensure we have enough data before accessing Data[1]
            int error_code = (Size > 1) ? (Data[1] % 100) : 0;
            const char *error_str = curl_multi_strerror((CURLMcode)error_code);
            if (error_str) {
                // Use the string to prevent unused variable warning
                (void)strlen(error_str);
            }
            res = CURLM_OK; // Dummy result
            break;

        case 5:
            // Mix of operations
            if (socket_fd >= 0) {
                res = curl_multi_assign(multi_handle, socket_fd, (void *)(uintptr_t)(Size > 1 ? Data[1] : 0));
                res = curl_multi_socket(multi_handle, socket_fd, &running_handles);
            }
            res = curl_multi_perform(multi_handle, &running_handles);
            break;

        default:
            res = CURLM_OK;
            break;
    }

    // Cleanup
    if (socket_fd >= 0) {
        close(socket_fd);
    }

    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}