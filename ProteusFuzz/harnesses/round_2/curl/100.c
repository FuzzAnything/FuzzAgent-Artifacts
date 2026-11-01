// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
#include <stdio.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_socket, curl_multi_perform
 * Note: Wcurl_* macros are compile-time type checkers, not runtime functions.
 * We exercise the underlying API functions they guard.
 */

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_xferinfo_callback(CURL *handle, double dltotal, double dlnow,
                                   double ultotal, double ulnow, void *userdata) {
    (void)handle;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
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

    /* Prepare a dummy file for file-based options if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Set some basic options to exercise type checking paths */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFODATA, (void*)Data);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /*
     * Extract parameters from fuzz input to diversify calls
     */
    curl_socket_t sock = -1;
    if (Size >= sizeof(curl_socket_t)) {
        sock = *(const curl_socket_t *)(Data);
        /* Ensure socket is non-negative for valid testing, or use -1 to test error paths */
        if (sock < 0) sock = -1;
    }

    int running_handles = 0;
    int *running_ptr = &running_handles;

    /*
     * Invoke curl_multi_socket (deprecated but still present)
     * This function may return CURLM_RECURSIVE_API_CALL if called from a callback,
     * or CURLM_OK/CURLM_BAD_HANDLE otherwise.
     */
    (void)curl_multi_socket(multi_handle, sock, running_ptr);

    /*
     * Invoke curl_multi_perform
     * This processes pending I/O and updates running_handles.
     */
    (void)curl_multi_perform(multi_handle, running_ptr);

    /*
     * Additional state exploration:
     * Try with NULL running_handles pointer (should be handled gracefully or return error)
     */
    (void)curl_multi_socket(multi_handle, sock, NULL);
    (void)curl_multi_perform(multi_handle, NULL);

    /*
     * Cleanup
     */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    /* Clean up dummy file */
    remove("./dummy_file");

    return 0;
}