// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_setopt, curl_multi_strerror, curl_multi_remove_handle,
 *          curl_multi_wakeup.
 * Note: Wcurl_* macros are compile-time checks and do not require runtime invocation.
 */

static CURLM *g_multi_handle = NULL;
static CURL *g_easy_handle = NULL;

static void setup_handles(void) {
    if (!g_multi_handle) {
        g_multi_handle = curl_multi_init();
    }
    if (!g_easy_handle) {
        g_easy_handle = curl_easy_init();
        if (g_multi_handle && g_easy_handle) {
            curl_multi_add_handle(g_multi_handle, g_easy_handle);
        }
    }
}

static void cleanup_handles(void) {
    if (g_multi_handle && g_easy_handle) {
        curl_multi_remove_handle(g_multi_handle, g_easy_handle);
    }
    if (g_easy_handle) {
        curl_easy_cleanup(g_easy_handle);
        g_easy_handle = NULL;
    }
    if (g_multi_handle) {
        curl_multi_cleanup(g_multi_handle);
        g_multi_handle = NULL;
    }
}

/* Dummy callback for CURLMOPT_PUSHFUNCTION */
static int dummy_push_cb(CURL *curl, struct curl_pushheaders *headers, void *userp) {
    (void)curl;
    (void)headers;
    (void)userp;
    return CURL_PUSH_OK;
}

/* Dummy callback for CURLMOPT_TIMERFUNCTION */
static int dummy_timer_cb(CURLM *multi, curl_off_t timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

/* Dummy callback for CURLMOPT_SOCKETFUNCTION */
static int dummy_socket_cb(CURLM *multi, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)multi;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_handles();

    if (!g_multi_handle || !g_easy_handle) {
        cleanup_handles();
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    const uint8_t *ptr = Data;
    uint8_t op = *ptr;
    ptr++;
    size_t remaining = Size - 1;

    /* Create a buffer from input for string options */
    char *str_buf = NULL;
    if (remaining > 0) {
        str_buf = (char *)malloc(remaining + 1);
        if (str_buf) {
            memcpy(str_buf, ptr, remaining);
            str_buf[remaining] = '\0';
        }
    }

    /* Fuzz curl_multi_setopt with various options */
    switch (op % 10) {
        case 0:
            /* CURLMOPT_MAXCONNECTS (long) */
            if (remaining >= sizeof(long)) {
                long val = *(long *)ptr;
                curl_multi_setopt(g_multi_handle, CURLMOPT_MAXCONNECTS, val);
            }
            break;
        case 1:
            /* CURLMOPT_MAX_HOST_CONNECTIONS (long) */
            if (remaining >= sizeof(long)) {
                long val = *(long *)ptr;
                curl_multi_setopt(g_multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, val);
            }
            break;
        case 2:
            /* CURLMOPT_PIPELINING (long) */
            if (remaining >= sizeof(long)) {
                long val = *(long *)ptr;
                curl_multi_setopt(g_multi_handle, CURLMOPT_PIPELINING, val);
            }
            break;
        case 3:
            /* CURLMOPT_PUSHFUNCTION (callback) */
            curl_multi_setopt(g_multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_cb);
            curl_multi_setopt(g_multi_handle, CURLMOPT_PUSHDATA, (void *)(long)ptr);
            break;
        case 4:
            /* CURLMOPT_SOCKETFUNCTION (callback) */
            curl_multi_setopt(g_multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_cb);
            curl_multi_setopt(g_multi_handle, CURLMOPT_SOCKETDATA, (void *)(long)ptr);
            break;
        case 5:
            /* CURLMOPT_TIMERFUNCTION (callback) */
            curl_multi_setopt(g_multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_cb);
            curl_multi_setopt(g_multi_handle, CURLMOPT_TIMERDATA, (void *)(long)ptr);
            break;
        case 6:
            /* CURLMOPT_MAXCONNECTS with invalid type (fuzzing type safety) */
            if (str_buf) {
                curl_multi_setopt(g_multi_handle, CURLMOPT_MAXCONNECTS, (long)(uintptr_t)str_buf);
            }
            break;
        case 7:
            /* CURLMOPT_PUSHFUNCTION with NULL */
            curl_multi_setopt(g_multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
            break;
        case 8:
            /* CURLMOPT_MAXCONNECTS with 0 */
            curl_multi_setopt(g_multi_handle, CURLMOPT_MAXCONNECTS, 0);
            break;
        case 9:
            /* CURLMOPT_MAXCONNECTS with -1 */
            curl_multi_setopt(g_multi_handle, CURLMOPT_MAXCONNECTS, -1);
            break;
    }

    /* Fuzz curl_multi_remove_handle */
    if ((op / 10) % 2 == 0) {
        curl_multi_remove_handle(g_multi_handle, g_easy_handle);
        /* Try to remove again to test idempotency */
        curl_multi_remove_handle(g_multi_handle, g_easy_handle);
        /* Re-add for further testing */
        curl_multi_add_handle(g_multi_handle, g_easy_handle);
    }

    /* Fuzz curl_multi_wakeup */
    if ((op / 20) % 2 == 0) {
        curl_multi_wakeup(g_multi_handle);
    }

    /* Fuzz curl_multi_strerror with various error codes */
    CURLMcode error_code = (CURLMcode)(op % 20);
    const char *err_str = curl_multi_strerror(error_code);
    if (err_str) {
        /* Use the string to prevent unused variable warning */
        (void)strlen(err_str);
    }

    /* Cleanup */
    cleanup_handles();
    free(str_buf);

    return 0;
}