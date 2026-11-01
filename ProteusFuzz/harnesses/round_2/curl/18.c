// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to associate with the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Extract inputs from fuzz data safely */
    uint32_t error_code = 0;
    uint32_t info_enum = 0;
    curl_off_t offt_value = 0;
    long timeout_ms = 0;

    if (Size >= 4) {
        memcpy(&error_code, Data, 4);
    }
    if (Size >= 8) {
        memcpy(&info_enum, Data + 4, 4);
    }
    
    /* Ensure we have enough bytes for curl_off_t before copying */
    if (Size >= 8 + sizeof(curl_off_t)) {
        memcpy(&offt_value, Data + 8, sizeof(curl_off_t));
    }

    if (Size >= 8 + sizeof(curl_off_t) + sizeof(long)) {
        memcpy(&timeout_ms, Data + 8 + sizeof(curl_off_t), sizeof(long));
    }

    /* 1. Fuzz curl_multi_strerror */
    const char *str = curl_multi_strerror((CURLMcode)error_code);
    (void)str;

    /* 2. Fuzz curl_multi_wakeup */
    CURLMcode wakeup_res = curl_multi_wakeup(multi_handle);
    (void)wakeup_res;

    /* 3. Fuzz curl_multi_get_offt */
    CURLMcode get_offt_res = curl_multi_get_offt(multi_handle, (CURLMinfo_offt)info_enum, &offt_value);
    (void)get_offt_res;

    /* 4. Fuzz curl_multi_timeout */
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_res;

    /* 5. Fuzz curl_multi_remove_handle */
    curl_multi_add_handle(multi_handle, easy_handle);
    CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_res;

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}