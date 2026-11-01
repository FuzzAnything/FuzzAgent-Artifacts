// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl_multi_wakeup and curl_multi_setopt.
 * Note: Wcurl_* macros are compile-time type checks and cannot be invoked at runtime.
 * This driver focuses on the runtime APIs: curl_multi_wakeup and curl_multi_setopt.
 */

static void dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
}

static int dummy_close_socket_callback(CURL *handle, curl_socket_t socketp, void *socketp_client) {
    (void)handle;
    (void)socketp;
    (void)socketp_client;
    return 0;
}

static int dummy_socket_callback(CURLM *multi, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)multi;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

/* curl_push_callback signature uses curl_hentry which is defined in curl.h in newer versions.
 * If not available, we define a compatible structure or use a workaround.
 * In standard libcurl, curl_hentry is defined as:
 * struct curl_hentry { char *name; char *value; };
 */
#ifndef CURLHENTRY_DEFINED
#define CURLHENTRY_DEFINED
struct curl_hentry {
    char *name;
    char *value;
};
typedef struct curl_hentry curl_hentry;
#endif

static int dummy_push_callback(CURL *parent, CURL *easy, size_t num_headers, curl_hentry *headers, void *userp) {
    (void)parent;
    (void)easy;
    (void)num_headers;
    (void)headers;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a dummy easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
        curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_callback);
        curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETDATA, (void *)Data);

        /* Add easy handle to multi handle */
        if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
            curl_easy_cleanup(easy_handle);
            easy_handle = NULL;
        }
    }

    /* Fuzz curl_multi_setopt with various options derived from input data */
    if (multi_handle) {
        /* Use input data to select options and values */
        if (Size > 0) {
            /* Try setting CURLMOPT_SOCKETFUNCTION */
            if (Data[0] % 2 == 0) {
                curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, (void *)Data);
            }

            /* Try setting CURLMOPT_TIMERFUNCTION */
            if (Size > 1 && Data[1] % 2 == 0) {
                curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, (void *)Data);
            }

            /* Try setting CURLMOPT_PUSHFUNCTION */
            if (Size > 2 && Data[2] % 2 == 0) {
                curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
                curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void *)Data);
            }

            /* Try setting max connects */
            if (Size > 3) {
                long max_connects = (long)(Data[3] & 0xFF);
                curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
            }

            /* Try setting pipelining */
            if (Size > 4) {
                long pipelining = (long)(Data[4] & 0xFF);
                curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
            }
        }

        /* Fuzz curl_multi_wakeup */
        /* Call wakeup multiple times to explore different states */
        for (int i = 0; i < 5 && Size > 0; i++) {
            curl_multi_wakeup(multi_handle);
        }
    }

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    curl_global_cleanup();
    return 0;
}