// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment
    // Initialize global curl
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    // Target 1: curl_global_trace
    // It expects a config string. We treat input as a potential config string.
    // We need a null-terminated string.
    char config_buf[256];
    size_t copy_len = (Size < sizeof(config_buf) - 1) ? Size : sizeof(config_buf) - 1;
    memcpy(config_buf, Data, copy_len);
    config_buf[copy_len] = '\0';

    // Call curl_global_trace.
    curl_global_trace(config_buf);

    // Target 2: curl_share_strerror
    // We can fuzz the error code. Use part of the input to generate a CURLSHcode.
    // Ensure Size >= 1 before accessing Data[0]
    CURLSHcode error_code = (CURLSHcode)(Data[0] % 256);
    const char *str = curl_share_strerror(error_code);
    if (str) {
        (void)str;
    }

    // Target 3, 4, 5, 6: curl_share_setopt, curl_easy_setopt, curl_share_cleanup
    // We need a CURLSH handle and a CURL handle.
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Prepare a safe string for CURLOPT_URL or other string options
    char url_buf[256];
    copy_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
    memcpy(url_buf, Data, copy_len);
    url_buf[copy_len] = '\0';

    // Fuzz curl_easy_setopt with various options
    // Option: CURLOPT_URL (String)
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);

    // Option: CURLOPT_COPYPOSTFIELDS (String)
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, url_buf);

    // Option: CURLOPT_TIMEOUT (Long)
    // Ensure Size >= 2 before accessing Data[1]
    long timeout = 0;
    if (Size >= 2) {
        timeout = (long)(Data[1] % 1000);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }

    // Option: CURLOPT_SHARE (CURLSH*)
    curl_easy_setopt(curl, CURLOPT_SHARE, share);

    // Fuzz curl_share_setopt
    // Ensure Size >= 4 before accessing Data[2] and Data[3]
    if (Size >= 4) {
        CURLSHoption share_opt = (CURLSHoption)(Data[2] % 10); 
        long share_val = (long)(Data[3] % 100);
        curl_share_setopt(share, share_opt, share_val);
    }

    // Cleanup must happen in reverse order of creation and usage
    // First, ensure the easy handle is not holding the share if it was set
    // But curl_easy_cleanup handles the reference count if CURLOPT_SHARE was set.
    
    // Target 6: curl_share_cleanup
    // We must ensure the share is not in use. Since we didn't perform any transfers,
    // it should be safe, but we must call it after the easy handle is cleaned up 
    // if the easy handle holds a reference to the share.
    // Actually, curl_easy_cleanup will decrement the reference count. 
    // If we call curl_share_cleanup before curl_easy_cleanup, it might fail if the handle is still active.
    // The correct order: cleanup easy handle first, then share.
    
    curl_easy_cleanup(curl);
    curl_share_cleanup(share);

    curl_global_cleanup();

    return 0;
}