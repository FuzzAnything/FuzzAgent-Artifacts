// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Define the callback types if not present in the specific curl version or headers.
 * curl_pushheaders_t is often an opaque struct or typedef'd in newer versions.
 * curl_pushcallback is the function pointer type.
 */
#ifndef CURL_PUSH_OK
#define CURL_PUSH_OK 0
#define CURL_PUSH_DENY 1
#endif

typedef struct curl_pushheaders_s curl_pushheaders_t;

typedef int (*curl_pushcallback)(CURL *easy_handle,
                                 curl_pushheaders_t *headers,
                                 void *userp);

static int push_callback(CURL *easy_handle,
                         curl_pushheaders_t *headers,
                         void *userp) {
    (void)easy_handle;
    (void)headers;
    (void)userp;
    return CURL_PUSH_DENY;
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
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

    /* Test curl_multi_setopt with PUSHFUNCTION */
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, (curl_pushcallback)push_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);

    /* Test curl_multi_add_handle (implied usage for remove_handle) */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Test curl_multi_remove_handle */
    curl_multi_remove_handle(multi_handle, easy_handle);
    /* Try removing again (should be safe) */
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Test curl_multi_assign */
    curl_socket_t sock = -1;
    void *sockp = (void *)Data;
    curl_multi_assign(multi_handle, sock, sockp);
    /* Assign with invalid socket */
    curl_multi_assign(multi_handle, 12345, sockp);

    /* Test curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Test curl_multi_strerror with various codes */
    const char *err_str = curl_multi_strerror(CURLM_OK);
    (void)err_str;
    err_str = curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)err_str;
    err_str = curl_multi_strerror(CURLM_INTERNAL_ERROR);
    (void)err_str;
    
    /* Fuzz with random error codes from input if size permits */
    if (Size >= sizeof(CURLMcode)) {
        CURLMcode fuzzed_code = *(const CURLMcode *)Data;
        err_str = curl_multi_strerror(fuzzed_code);
        (void)err_str;
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}