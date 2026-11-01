// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback to satisfy type requirements */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize global curl environment */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Prepare dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Set some basic options to make the handle usable */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE, (long)Size);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_BAD;

    /* Fuzz curl_multi_perform */
    curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket_all */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket with various socket values */
    if (Size >= 4) {
        s = *(const int *)(Data);
        curl_multi_socket(multi_handle, s, &running_handles);
    }

    /* Try to trigger type-checking macros by setting options with fuzzed data */
    /* Note: These macros are compile-time checks, so we just ensure valid calls */
    struct curl_slist *headers = NULL;
    if (Size > 10) {
        char *header_str = malloc(Size);
        if (header_str) {
            memcpy(header_str, Data, Size);
            header_str[Size - 1] = '\0';
            headers = curl_slist_append(headers, header_str);
            curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
            free(header_str);
        }
    }

    /* Test curl_easy_getinfo with various info types */
    char *str_ptr = NULL;
    long long_val = 0;
    double double_val = 0.0;
    curl_off_t off_val = 0;
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    struct curl_slist *slist_ptr = NULL;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;

    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &str_ptr);
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &double_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_UPLOAD, &off_val);
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &socket_val);

    /* Cleanup */
    if (headers) {
        curl_slist_free_all(headers);
    }

    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}