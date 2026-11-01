// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type checks */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return size * nmemb;
}

static int dummy_closesocket_cb(CURL *handle, curl_socket_t socket, void *socketp) {
    (void)handle; (void)socket; (void)socketp;
    return 0;
}

static int dummy_chunk_bgn_cb(CURL *curl, void *chunk, void *ptr, int first, int total) {
    (void)curl; (void)chunk; (void)ptr; (void)first; (void)total;
    return 0;
}

static int dummy_ssl_ctx_cb(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl; (void)ssl_ctx; (void)userptr;
    return 0;
}

static int dummy_socket_cb(CURLM *multi, CURL *easy, curl_socket_t s, int what, int action, void *userp) {
    (void)multi; (void)easy; (void)s; (void)what; (void)action; (void)userp;
    return 0;
}

static int dummy_timer_cb(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

static int dummy_push_cb(CURL *easy, const struct curl_pushheaders *headers, void *userp) {
    (void)easy; (void)headers; (void)userp;
    return 0;
}

static int dummy_notify_cb(CURLM *multi, CURL *easy, CURLMSG msg, void *userp) {
    (void)multi; (void)easy; (void)msg; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Parse input to diversify states */
    int running_handles = 0;
    curl_socket_t sock = CURL_SOCKET_TIMEOUT;
    
    if (Size > 0) {
        running_handles = Data[0];
    }
    if (Size > 1) {
        sock = (curl_socket_t)Data[1];
    }
    
    /* Set options to trigger type checking macros and internal logic */
    /* CURLOPT_CLOSESOCKETFUNCTION */
    curl_easy_setopt(easy, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_cb);
    curl_easy_setopt(easy, CURLOPT_CLOSESOCKETDATA, (void*)Data);

    /* CURLOPT_CHUNK_BGN_FUNCTION */
    curl_easy_setopt(easy, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_cb);
    curl_easy_setopt(easy, CURLOPT_CHUNK_DATA, (void*)Data);

    /* CURLOPT_SSL_CTX_FUNCTION */
    curl_easy_setopt(easy, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);
    curl_easy_setopt(easy, CURLOPT_SSL_CTX_DATA, (void*)Data);

    /* Basic setup for multi handle */
    curl_multi_add_handle(multi, easy);

    /* Fuzz curl_multi_socket (deprecated but available) */
    /* This function is deprecated but still callable for fuzzing legacy paths */
    (void)curl_multi_socket(multi, sock, &running_handles);

    /* Fuzz curl_multi_socket_all (deprecated but available) */
    (void)curl_multi_socket_all(multi, &running_handles);

    /* Fuzz curl_multi_setopt with various callbacks */
    /* Fixed: Use CURLMOPT_SOCKETFUNCTION instead of CURLOPTSOCKETFUNCTION */
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, dummy_socket_cb);
    curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, (void*)Data);

    /* Fixed: Use CURLMOPT_TIMERFUNCTION instead of CURLOPT_MULTI_TIMERFUNCTION */
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, dummy_timer_cb);
    curl_multi_setopt(multi, CURLMOPT_TIMERDATA, (void*)Data);

    /* Fixed: Use CURLMOPT_PUSHFUNCTION instead of CURLOPT_MULTI_PUSHFUNCTION */
    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_cb);
    curl_multi_setopt(multi, CURLMOPT_PUSHDATA, (void*)Data);

    /* Fixed: Use CURLMOPT_NOTIFYFUNCTION instead of CURLOPT_MULTI_NOTIFYFUNCTION */
    curl_multi_setopt(multi, CURLMOPT_NOTIFYFUNCTION, dummy_notify_cb);
    curl_multi_setopt(multi, CURLMOPT_NOTIFYDATA, (void*)Data);

    /* Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    return 0;
}