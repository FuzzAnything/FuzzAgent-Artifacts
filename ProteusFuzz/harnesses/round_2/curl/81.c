// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* Suppress deprecation warnings for the target functions */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode res;
    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_BAD;
    
    if (Size < 2) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

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

    /* Set a dummy URL to ensure the handle is valid for multi operations */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    /* CURLOPT_RETURNTRANSFER is not available in this build, use NULL write callback instead */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, NULL);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_multi_socket */
    /* Use fuzz data to determine socket value, but keep it within safe bounds for testing */
    if (Size > 0) {
        s = (curl_socket_t)(Data[0] & 0x7F); /* Ensure non-negative for socket test */
    }
    
    /* Invoke deprecated curl_multi_socket */
    res = curl_multi_socket(multi_handle, s, &running_handles);
    (void)res; /* Suppress unused result warning */

    /* Fuzz curl_multi_socket_all */
    /* Invoke deprecated curl_multi_socket_all */
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res; /* Suppress unused result warning */

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}
#pragma GCC diagnostic pop