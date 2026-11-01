// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
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

/* Dummy callback to satisfy type requirements if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* 1. Test curl_multi_strerror with various inputs derived from Data */
    if (Size >= 2) {
        CURLMcode code = (CURLMcode)(Data[0] | (Data[1] << 8));
        const char *msg = curl_multi_strerror(code);
        if (msg) {
            /* Consume the string to prevent dead code elimination */
            volatile size_t len = strlen(msg);
            (void)len;
        }
    }

    /* 2. Test curl_multi_remove_handle */
    /* Add handle first to ensure it can be removed */
    if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
        CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)remove_res;
    }

    /* 3. Test curl_multi_timeout */
    long timeout_ms = 0;
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_res;

    /* 4. Test curl_multi_get_offt */
    curl_off_t offt_val = 0;
    /* Use CURLMINFO_ACTIVE if defined, otherwise use 0 or a safe default */
#ifdef CURLMINFO_ACTIVE
    CURLMcode offt_res = curl_multi_get_offt(multi_handle, CURLMINFO_ACTIVE, &offt_val);
#else
    CURLMcode offt_res = curl_multi_get_offt(multi_handle, (CURLMinfo_offt)0, &offt_val);
#endif
    (void)offt_res;

    /* 5. Test curl_multi_wakeup */
    CURLMcode wakeup_res = curl_multi_wakeup(multi_handle);
    (void)wakeup_res;

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global environment */
    curl_global_cleanup();

    return 0;
}