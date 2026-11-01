// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Suppress deprecation warnings for testing deprecated functions */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
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

    /* Setup basic options for the easy handle to ensure valid state */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    
    /* Use fuzzing data to set various options safely */
    if (Size > 10) {
        /* Use a portion of data as a string option (e.g., URL or User-Agent) */
        char *url_buf = malloc(Size - 10);
        if (url_buf) {
            memcpy(url_buf, Data + 10, Size - 10);
            url_buf[Size - 10] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
            free(url_buf);
        }
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t sock = -1;
    int ev_bitmask = 0;
    void *sockp = (void *)(uintptr_t)Data[0];

    /* Fuzz curl_multi_perform */
    (void)curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket (deprecated) */
    (void)curl_multi_socket(multi_handle, sock, &running_handles);

    /* Fuzz curl_multi_socket_all (deprecated) */
    (void)curl_multi_socket_all(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket_action */
    if (Size > 1) {
        ev_bitmask = Data[1];
        (void)curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    }

    /* Fuzz curl_multi_assign */
    (void)curl_multi_assign(multi_handle, sock, sockp);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}

#pragma GCC diagnostic pop