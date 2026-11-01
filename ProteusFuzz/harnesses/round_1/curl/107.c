// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
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
// curl_multi_strerror at strerror.c:326:13 in multi.h
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

/* Dummy callback functions to satisfy type checking macros and setopt requirements */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_xferinfo_cb(CURL *handle, double dltotal, double dlnow, double ultotal, double ulnow, void *userptr) {
    (void)handle; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow; (void)userptr;
    return 0;
}

static int dummy_sockopt_cb(CURL *handle, curl_socket_t fd, curlsocktype purpose) {
    (void)handle; (void)fd; (void)purpose;
    return CURL_SOCKOPT_OK;
}

/* Forward declaration for curl_hentry to avoid compilation error if struct is not fully visible */
struct curl_hentry;

static int dummy_push_cb(CURL *parent, CURL *easy, size_t num_headers, struct curl_hentry *headers, void *userp) {
    (void)parent; (void)easy; (void)num_headers; (void)headers; (void)userp;
    return 1; /* Accept push */
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Initialize easy handle with basic options to avoid immediate errors */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Fuzz curl_multi_strerror */
    /* Interpret first byte as error code */
    CURLMcode error_code = (CURLMcode)Data[0];
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        /* Consume the string to prevent unused variable warnings and explore state */
        size_t len = strlen(error_str);
        if (len > 0 && len < 100) {
            volatile char dummy = error_str[0];
            (void)dummy;
        }
    }

    /* Fuzz curl_multi_setopt */
    /* Interpret bytes to determine option and value */
    if (Size > 2) {
        CURLMoption option = (CURLMoption)(Data[1] & 0xFF);
        
        /* Try setting various options based on available data */
        switch (option) {
            case CURLMOPT_PUSHFUNCTION:
                curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_cb);
                curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void*)(uintptr_t)(Data[2]));
                break;
            case CURLMOPT_SOCKETFUNCTION:
                /* We don't have a socket callback defined, so skip or use NULL */
                break;
            case CURLMOPT_TIMERFUNCTION:
                /* Skip timer function as it's complex to implement correctly for fuzzing */
                break;
            default:
                /* Try setting a long value option if possible */
                if (Size > 4) {
                    long val = (long)(Data[2] | (Data[3] << 8) | (Data[4] << 16));
                    curl_multi_setopt(multi_handle, option, val);
                } else {
                    curl_multi_setopt(multi_handle, option, 0);
                }
                break;
        }
    }

    /* Fuzz curl_multi_wakeup */
    /* Always try to wakeup, it's safe to call multiple times */
    curl_multi_wakeup(multi_handle);

    /* Explore state by performing a poll cycle (non-blocking) */
    int running_handles = 0;
    /* curl_multi_poll signature: curl_multi_poll(CURLM *multi_handle, struct curl_waitfd extra_fds[], unsigned int extra_nfds, int timeout_ms, int *ret) */
    curl_multi_poll(multi_handle, NULL, 0, 0, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}