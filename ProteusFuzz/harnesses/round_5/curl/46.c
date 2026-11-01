// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* Fuzz driver for curl multi-handle API functions */

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode res;
    const char *err_str;
    curl_socket_t sock = 0;
    void *sockp = NULL;

    /* Initialize curl global library once (idempotent) */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Optional: set a dummy URL to avoid some internal errors */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);

    /* Fuzz curl_multi_add_handle */
    res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res == CURLM_OK) {
        /* Try adding again to trigger CURLM_ADDED_ALREADY */
        res = curl_multi_add_handle(multi_handle, easy_handle);
        (void)res;
    }

    /* Fuzz curl_multi_setopt with various options */
    /* Set PUSHFUNCTION (requires callback, but we can pass NULL to test error handling) */
    res = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    (void)res;

    /* Try setting other options if available via fuzz data interpretation */
    if (Size > 0) {
        int option_val = Data[0];
        /* We can't directly set arbitrary options without knowing the enum,
           but we can try a few known ones safely or just ignore for now. */
        (void)option_val;
    }

    /* Fuzz curl_multi_assign */
    /* Use a dummy socket value; in real usage, this would be a real socket */
    if (Size > 1) {
        sock = (curl_socket_t)Data[1];
    }
    /* sockp can point to fuzz data or NULL */
    if (Size > 2) {
        sockp = (void *)(Data + 2);
    }
    res = curl_multi_assign(multi_handle, sock, sockp);
    (void)res;

    /* Fuzz curl_multi_remove_handle */
    res = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)res;

    /* Try removing again (should be safe) */
    res = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)res;

    /* Fuzz curl_multi_wakeup */
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    /* Fuzz curl_multi_strerror with various error codes */
    for (int i = 0; i < 10 && Size > (size_t)i; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        err_str = curl_multi_strerror(code);
        if (err_str) {
            /* Just ensure it doesn't crash; we don't need to use the string */
            (void)err_str;
        }
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}