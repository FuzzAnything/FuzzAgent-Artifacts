// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

/* Dummy push callback to satisfy type requirements */
static int dummy_push_callback(CURL *parent, CURL *easy_handle,
                               size_t num_headers, struct curl_hentry *headers,
                               void *userp) {
    (void)parent;
    (void)easy_handle;
    (void)num_headers;
    (void)headers;
    (void)userp;
    return CURL_PUSH_DENY;
}

/* Dummy socket callback */
static int dummy_socket_callback(CURLM *multi_handle, curl_socket_t s,
                                 int what, void *userp, void *socketp) {
    (void)multi_handle;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

/* Dummy timer callback */
static int dummy_timer_callback(CURLM *multi_handle, long timeout_ms,
                                void *userp) {
    (void)multi_handle;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

/* Dummy notify callback */
static int dummy_notify_callback(CURLM *multi_handle, void *userp) {
    (void)multi_handle;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Initialize a dummy easy handle to add to multi if needed */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* Use input data to drive fuzzing logic */
    unsigned int option_choice = Data[0] % 10;
    unsigned int error_code = (Size > 1) ? Data[1] : 0;

    /* Fuzz curl_multi_setopt with various options */
    switch (option_choice) {
        case 0:
            /* Set push function */
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
            break;
        case 1:
            /* Set push function to NULL (deny all) */
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
            break;
        case 2:
            /* Set socket callback */
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
            break;
        case 3:
            /* Set timer callback */
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
            break;
        case 4:
            /* Set notify callback */
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
            break;
        case 5:
            /* Set max connects */
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Data[0] % 100));
            break;
        case 6:
            /* Set pipe buffer size */
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Data[0] % 2));
            break;
        case 7:
            /* Set max hosts */
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Data[0] % 10));
            break;
        case 8:
            /* Set max connects per host */
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, (long)(Data[0] % 10));
            break;
        case 9:
            /* Set content length penalty */
            curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, (long)(Data[0] % 1000));
            break;
        default:
            break;
    }

    /* Fuzz curl_multi_strerror with various error codes */
    const char *error_str = curl_multi_strerror((CURLMcode)error_code);
    if (error_str) {
        /* Use the string to avoid unused variable warning */
        (void)strlen(error_str);
    }

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Cleanup */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}