// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
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

/* Push callback for curl_multi_setopt CURLMOPT_PUSHFUNCTION */
static int push_cb(CURL *parent, CURL *easy, size_t num_headers,
                   struct curl_header *headers, void *userp) {
    (void)parent;
    (void)easy;
    (void)num_headers;
    (void)headers;
    (void)userp;
    /* Approve all pushes for fuzzing purposes */
    return 0;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl_global if not already done (idempotent) */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /*
     * Fuzzing Logic:
     * We use the input data to drive various option settings and function calls.
     */

    /* 1. Test curl_multi_setopt with CURLMOPT_PUSHFUNCTION */
    /* We set the callback to our static function */
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, push_cb);
    /* Set userp to NULL or a pointer into the fuzz input (if safe) */
    if (Size > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void*)(uintptr_t)Data);
    }

    /* 2. Test curl_multi_wakeup */
    /* This function is designed to wake up a sleeping poll, but calling it
     * on a handle that isn't polling should be safe and exercise the handle
     * validation logic. */
    curl_multi_wakeup(multi_handle);

    /* 3. Exercise curl_easy_setopt with various types using input data */
    if (Size > 0) {
        /* Set URL if we have enough data */
        if (Size > 5) {
            char url_buf[256];
            size_t url_len = (Size > 255) ? 255 : Size;
            memcpy(url_buf, Data, url_len);
            url_buf[url_len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
        }

        /* Set a long option */
        if (Size > 1) {
            long timeout = (long)Data[1];
            curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
        }

        /* Set a string option (User-Agent) */
        /* Ensure we have at least 3 bytes to avoid reading past Data+2 */
        if (Size > 2) {
            char agent_buf[64];
            /* Calculate length safely: max 63 chars + null terminator */
            size_t available = Size - 2;
            size_t agent_len = (available > 63) ? 63 : available;
            memcpy(agent_buf, Data + 2, agent_len);
            agent_buf[agent_len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, agent_buf);
        }

        /* Set error buffer */
        char error_buf[CURL_ERROR_SIZE];
        memset(error_buf, 0, CURL_ERROR_SIZE);
        curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buf);

        /* Set callback data (void*) */
        if (Size > 0) {
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void*)(uintptr_t)Data);
        }
    }

    /* 4. Exercise curl_easy_getinfo with various types */
    char *str_info = NULL;
    long long_info = 0;
    double double_info = 0.0;
    struct curl_slist *slist_info = NULL;
    curl_socket_t socket_info = CURL_SOCKET_BAD;
    curl_off_t off_t_info = 0;

    if (easy_handle) {
        /* String info */
        curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &str_info);
        /* Long info */
        curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_info);
        /* Double info */
        curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &double_info);
        /* Socket info */
        curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &socket_info);
        /* Off_t info */
        curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &off_t_info);
        /* Slist info (might return NULL if no data, which is fine) */
        curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &slist_info);
        if (slist_info) {
            curl_slist_free_all(slist_info);
        }
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}