// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURLMcode res;
    curl_off_t offt_value;
    long timeout_ms;
    CURLMinfo_offt info_enum;
    const char *error_str;

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Parse input to determine operations
    size_t idx = 0;

    // 1. Fuzz curl_multi_get_offt
    if (idx < Size) {
        info_enum = (CURLMinfo_offt)Data[idx++];
    } else {
        info_enum = CURLMINFO_NONE;
    }

    res = curl_multi_get_offt(multi_handle, info_enum, &offt_value);
    // Check return value to explore error paths
    (void)res;

    // 2. Fuzz curl_multi_strerror
    if (idx < Size) {
        CURLMcode code = (CURLMcode)Data[idx++];
        error_str = curl_multi_strerror(code);
        // Use the string to prevent compiler optimization of unused result
        if (error_str) {
            volatile int len = strlen(error_str);
            (void)len;
        }
    }

    // 3. Fuzz curl_multi_timeout
    if (idx < Size) {
        res = curl_multi_timeout(multi_handle, &timeout_ms);
        (void)res;
    }

    // 4. Fuzz curl_multi_wakeup
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}