// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type checking */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_closesocket_callback(void *handle, curl_socket_t socket) {
    (void)handle; (void)socket;
    return 0;
}

static int dummy_interleave_callback(CURL *easy_handle, int ev, void *userp) {
    (void)easy_handle; (void)ev; (void)userp;
    return 0;
}

static int dummy_push_callback(CURL *easy_handle, void *userp) {
    (void)easy_handle; (void)userp;
    return 0;
}

static int dummy_socket_callback(CURLM *multi_handle, curl_socket_t s, int what, void *userp) {
    (void)multi_handle; (void)s; (void)what; (void)userp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi_handle, long timeout_ms, void *userp) {
    (void)multi_handle; (void)timeout_ms; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize global curl environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set some basic options to ensure the handle is valid */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);

    /* Test Wcurl_easy_setopt_err_close_socket_cb via curl_easy_setopt */
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETDATA, (void *)Data);

    /* Test Wcurl_easy_setopt_err_interleave_cb via curl_easy_setopt */
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEDATA, (void *)Data);

    /* Test Wcurl_easy_setopt_err_error_buffer via curl_easy_setopt */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, CURL_ERROR_SIZE);
    curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buffer);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Test Wcurl_multi_setopt_err_pushcb via curl_multi_setopt using correct constant */
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);

    /* Test Wcurl_multi_setopt_err_socketcb via curl_multi_setopt */
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);

    /* Test Wcurl_multi_setopt_err_timercb via curl_multi_setopt */
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);

    /* Prepare running_handles variable */
    int running_handles = 0;

    /* Fuzz curl_multi_socket with various socket values derived from input */
    curl_socket_t socket_fd = CURL_SOCKET_BAD;
    if (Size >= sizeof(curl_socket_t)) {
        socket_fd = *(const curl_socket_t *)Data;
        /* Invoke deprecated curl_multi_socket */
        curl_multi_socket(multi_handle, socket_fd, &running_handles);
    }

    /* Fuzz curl_multi_socket_all */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}