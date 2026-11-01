// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_all at multi.c:3302:11 in multi.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback to satisfy CURLOPT_CLOSESOCKETFUNCTION requirement */
static int dummy_close_socket_callback(curl_socket_t sockfd, void *clientp) {
    (void)sockfd;
    (void)clientp;
    return 0;
}

/* Dummy write callback */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

/* Dummy read callback */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
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

    /* Set a basic URL to ensure the handle is valid for transfer */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    
    /* Test the callback type safety macro by setting the close socket callback */
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETDATA, (void *)Data);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    
    /* Interpret input data to diversify function calls */
    if (Size >= 1) {
        sockfd = (curl_socket_t)(Data[0]);
    }
    if (Size >= 2) {
        int ev_bitmask = (int)(Data[1]);
        /* curl_multi_socket_action is the primary target for socket action */
        (void)curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
    }

    /* Test curl_multi_assign with data from input */
    if (Size >= 3) {
        void *sockp = (void *)(uintptr_t)(Data[2]);
        (void)curl_multi_assign(multi_handle, sockfd, sockp);
    }

    /* Test curl_multi_perform */
    (void)curl_multi_perform(multi_handle, &running_handles);

    /* Test deprecated curl_multi_socket */
    if (Size >= 4) {
        curl_socket_t s = (curl_socket_t)(Data[3]);
        (void)curl_multi_socket(multi_handle, s, &running_handles);
    }

    /* Test deprecated curl_multi_socket_all */
    (void)curl_multi_socket_all(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}