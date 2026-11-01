// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper to safely create a CURL handle if input data allows */
static CURL *create_easy_handle(const uint8_t *Data, size_t Size) {
    if (Size < 1) return NULL;
    if (Data[0] % 2 == 0) {
        return curl_easy_init();
    }
    return NULL;
}

/* Helper to safely create a CURLM handle */
static CURLM *create_multi_handle(void) {
    return curl_multi_init();
}

/* Dummy callback for CURLOPT_XFERINFOFUNCTION to satisfy type checks */
static int dummy_xferinfo_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    return 0;
}

/* Dummy callback for CURLOPT_WRITEFUNCTION */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode mcode;
    CURLcode ccode;
    char error_buffer[CURL_ERROR_SIZE];
    const char *str_err;
    long long_val;
    double double_val;
    curl_off_t off_val;
    curl_socket_t sock_val;
    struct curl_slist *slist = NULL;
    struct curl_tlssessioninfo tls_info;
    struct curl_certinfo cert_info;
    char *string_ptr = NULL;

    /* Initialize multi handle */
    multi_handle = create_multi_handle();
    if (!multi_handle) {
        return 0;
    }

    /* Initialize easy handle based on input randomness */
    easy_handle = create_easy_handle(Data, Size);
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* 1. Test Wcurl_easy_setopt_err_error_buffer (via curl_easy_setopt) */
    memset(error_buffer, 0, CURL_ERROR_SIZE);
    ccode = curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buffer);
    (void)ccode;

    /* 2. Test curl_multi_strerror */
    /* Fuzz different error codes derived from input */
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLMcode test_code = (CURLMcode)(Data[i] + (i * 5));
        str_err = curl_multi_strerror(test_code);
        if (str_err) {
            /* Consume the string to avoid unused warnings */
            volatile int len = strlen(str_err);
            (void)len;
        }
    }

    /* 3. Test curl_multi_setopt */
    /* Set various options to explore states */
    mcode = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)Data[0]);
    (void)mcode;

    /* Set a callback if data suggests it */
    if (Size > 1 && (Data[1] % 3) == 0) {
        mcode = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
        (void)mcode;
    }

    /* 4. Test curl_multi_remove_handle */
    /* First add the handle, then remove it */
    mcode = curl_multi_add_handle(multi_handle, easy_handle);
    if (mcode == CURLM_OK) {
        mcode = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)mcode;
    }

    /* Test removing again (should be safe/handle already removed) */
    mcode = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)mcode;

    /* 5. Test Wcurl_easy_setopt_err_xferinfo_cb (via curl_easy_setopt) */
    ccode = curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);
    (void)ccode;

    /* 6. Test curl_multi_wakeup */
    mcode = curl_multi_wakeup(multi_handle);
    (void)mcode;

    /* Additional fuzzing: diverse curl_easy_setopt calls using input data */
    if (Size > 2) {
        /* Set URL if possible (dummy) */
        char url_buf[256];
        size_t url_len = (Size > 256) ? 256 : Size;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';
        ccode = curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
        (void)ccode;

        /* Set write function */
        ccode = curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
        (void)ccode;

        /* Set long option */
        long opt_val = (long)Data[2];
        ccode = curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, opt_val);
        (void)ccode;
    }

    /* Additional fuzzing: curl_easy_getinfo calls to trigger type checks */
    ccode = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
    (void)ccode;

    ccode = curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &double_val);
    (void)ccode;

    ccode = curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
    (void)ccode;

    ccode = curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &sock_val);
    (void)ccode;

    ccode = curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &string_ptr);
    (void)ccode;
    if (string_ptr) {
        /* string_ptr is managed by curl, do not free */
    }

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    return 0;
}