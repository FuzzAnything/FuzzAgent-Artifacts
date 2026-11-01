// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <fcntl.h>
#include <unistd.h>

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
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

    // Prepare a dummy file if needed
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Configure easy handle with various options using fuzz data
    char *safe_url = NULL;
    if (Size > 0 && Data[0] == 0) {
        safe_url = "http://example.com";
    } else {
        safe_url = malloc(Size + 1);
        if (safe_url) {
            memcpy(safe_url, Data, Size);
            safe_url[Size] = '\0';
        }
    }

    if (safe_url) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, safe_url);
        if (safe_url != "http://example.com") {
            free(safe_url);
        }
    }

    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t socket_fd = -1;
    int ev_bitmask = 0;

    // Fuzz curl_multi_socket_all
    curl_multi_socket_all(multi_handle, &running_handles);

    // Fuzz curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Fuzz curl_multi_socket_action with varying socket and event bitmask
    // Ensure we have enough data before accessing indices 0, 1, 2
    if (Size >= 3) {
        socket_fd = (curl_socket_t)(Data[0] | (Data[1] << 8));
        ev_bitmask = Data[2] & 0xFF;
    } else if (Size >= 2) {
        socket_fd = (curl_socket_t)(Data[0] | (Data[1] << 8));
        ev_bitmask = 1;
    } else if (Size >= 1) {
        socket_fd = (curl_socket_t)Data[0];
        ev_bitmask = 1;
    } else {
        socket_fd = 0;
        ev_bitmask = 1;
    }
    curl_multi_socket_action(multi_handle, socket_fd, ev_bitmask, &running_handles);

    // Fuzz curl_multi_socket (deprecated)
    curl_multi_socket(multi_handle, socket_fd, &running_handles);

    // Fuzz curl_multi_assign
    void *sockp = NULL;
    if (Size >= 2) {
        sockp = (void *)(size_t)(Data[0] | (Data[1] << 8));
    } else if (Size >= 1) {
        sockp = (void *)(size_t)Data[0];
    } else {
        sockp = (void *)0;
    }
    curl_multi_assign(multi_handle, socket_fd, sockp);

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}