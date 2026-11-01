// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy read callback to satisfy CURLOPT_READFUNCTION type checks */
static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userp) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userp;
    return 0;
}

/* Dummy write callback */
static size_t dummy_write_callback(char *buffer, size_t size, size_t nitems, void *userp) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize global curl environment once if not already done */
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        initialized = 1;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Create an easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Set a dummy URL to avoid immediate errors in some paths */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);
    
    /* Set callbacks to satisfy type checking macros implicitly */
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Extract data from fuzz input for diverse testing */
    int running_handles = 0;
    curl_socket_t socket_fd = -1;
    void *sockp = NULL;
    long long_val = 0;
    char *str_val = NULL;
    struct curl_slist *slist_val = NULL;
    struct curl_certinfo *certinfo_val = NULL;
    struct curl_tlssessioninfo *tlsinfo_val = NULL;
    curl_socket_t socket_val = -1;
    curl_off_t off_val = 0;
    double double_val = 0.0;

    /* Interpret input bytes to drive function calls safely */
    if (Size > 0) {
        socket_fd = (curl_socket_t)(Data[0] ^ 0xFF);
    }
    if (Size > 1) {
        running_handles = (int)(Data[1] % 10);
    }
    if (Size > 2) {
        long_val = (long)(Data[2] * 100);
    }
    if (Size > 3) {
        off_val = (curl_off_t)(Data[3] * 1000);
    }
    if (Size > 4) {
        double_val = (double)Data[4];
    }

    /* Test curl_multi_perform */
    curl_multi_perform(multi_handle, &running_handles);

    /* Test curl_multi_socket (deprecated but valid for fuzzing) */
    curl_multi_socket(multi_handle, socket_fd, &running_handles);

    /* Test curl_multi_assign */
    /* We use a simple pointer derived from data if size allows, else NULL */
    if (Size > 5) {
        sockp = (void *)(uintptr_t)(Data[5] * 100);
    }
    curl_multi_assign(multi_handle, socket_fd, sockp);

    /* Test curl_easy_getinfo with various types to trigger type checking logic */
    
    /* String info */
    if (Size > 6) {
        char *tmp_str = NULL;
        curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &tmp_str);
        (void)tmp_str;
    }

    /* Long info */
    if (Size > 7) {
        long tmp_long = 0;
        curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &tmp_long);
        (void)tmp_long;
    }

    /* Double info */
    if (Size > 8) {
        double tmp_double = 0.0;
        curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &tmp_double);
        (void)tmp_double;
    }

    /* Socket info */
    if (Size > 9) {
        curl_socket_t tmp_sock = -1;
        curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &tmp_sock);
        (void)tmp_sock;
    }

    /* Off_t info */
    if (Size > 10) {
        curl_off_t tmp_off = 0;
        curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD_T, &tmp_off);
        (void)tmp_off;
    }

    /* Slist info (CURLINFO_COOKIELIST) */
    if (Size > 11) {
        struct curl_slist *tmp_slist = NULL;
        curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &tmp_slist);
        if (tmp_slist) {
            curl_slist_free_all(tmp_slist);
        }
    }

    /* Certinfo info */
    if (Size > 12) {
        struct curl_certinfo *tmp_cert = NULL;
        curl_easy_getinfo(easy_handle, CURLINFO_CERTINFO, &tmp_cert);
        (void)tmp_cert;
    }

    /* TlsSessionInfo info */
    if (Size > 13) {
        struct curl_tlssessioninfo *tmp_tls = NULL;
        curl_easy_getinfo(easy_handle, CURLINFO_TLS_SESSION, &tmp_tls);
        (void)tmp_tls;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}