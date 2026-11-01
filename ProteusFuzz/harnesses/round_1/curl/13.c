// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
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

/* Static helper to create a dummy easy handle for multi operations */
static CURL *create_dummy_easy_handle(const uint8_t *data, size_t size) {
    CURL *handle = curl_easy_init();
    if (!handle) return NULL;

    /* Set a simple URL to ensure the handle is valid */
    if (size > 10 && data[0] != 0) {
        char url[256];
        size_t len = (size < 255) ? size : 255;
        memcpy(url, data, len);
        url[len] = '\0';
        curl_easy_setopt(handle, CURLOPT_URL, url);
    } else {
        curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    }

    /* Set a dummy write callback to avoid errors during transfer setup */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);

    return handle;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = create_dummy_easy_handle(Data, Size);
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Add easy handle to multi handle */
    CURLMcode mcode = curl_multi_add_handle(multi_handle, easy_handle);
    if (mcode != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Fuzz curl_multi_wait */
    struct curl_waitfd extra_fds[2];
    int ret = 0;
    int timeout_ms = (Size > 0) ? (int)Data[0] : 1000;
    unsigned int extra_nfds = (Size > 1) ? (unsigned int)Data[1] % 2 : 0;

    if (extra_nfds > 0) {
        extra_fds[0].fd = -1; /* Invalid fd for fuzzing safety */
        extra_fds[0].events = 0;
        extra_fds[0].revents = 0;
    }

    mcode = curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    (void)mcode; /* Use return value to explore states */
    (void)ret;

    /* Fuzz curl_multi_strerror with various error codes */
    CURLMcode test_error = (CURLMcode)(Size % 100);
    const char *err_str = curl_multi_strerror(test_error);
    if (err_str) {
        /* Ensure string is non-null and accessible */
        (void)err_str[0];
    }

    /* Fuzz curl_multi_waitfds */
    unsigned int fd_count = 0;
    struct curl_waitfd *ufds = NULL;
    unsigned int size_fds = (Size > 2) ? (unsigned int)Data[2] : 0;

    if (size_fds > 0) {
        ufds = malloc(sizeof(struct curl_waitfd) * size_fds);
        if (ufds) {
            memset(ufds, 0, sizeof(struct curl_waitfd) * size_fds);
        }
    }

    mcode = curl_multi_waitfds(multi_handle, ufds, size_fds, &fd_count);
    if (ufds) {
        free(ufds);
    }

    /* Fuzz curl_multi_remove_handle */
    mcode = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)mcode;

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}