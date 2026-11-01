// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    // Create a CURL handle
    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    // Fuzz curl_global_trace with the input data as a config string
    // Ensure null-termination for the config string
    char *config_str = (char *)malloc(Size + 1);
    if (config_str) {
        memcpy(config_str, Data, Size);
        config_str[Size] = '\0';
        curl_global_trace(config_str);
        free(config_str);
    }

    // Set a dummy URL to avoid errors during perform
    res = curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    if (res != CURLE_OK) {
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        return 0;
    }

    // Fuzz curl_easy_setopt with various options
    // Ensure null-termination for string options
    if (Size > 0) {
        char *opt_value = (char *)malloc(Size + 1);
        if (opt_value) {
            memcpy(opt_value, Data, Size);
            opt_value[Size] = '\0';

            // Try setting different options with the fuzzed data
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, opt_value);
            curl_easy_setopt(handle, CURLOPT_COPYPOSTFIELDS, opt_value);
            curl_easy_setopt(handle, CURLOPT_USERAGENT, opt_value);
            curl_easy_setopt(handle, CURLOPT_REFERER, opt_value);

            free(opt_value);
        }
    }

    // Fuzz curl_easy_pause with different bitmasks
    int pause_masks[] = {
        CURLPAUSE_RECV,
        CURLPAUSE_SEND,
        CURLPAUSE_RECV | CURLPAUSE_SEND,
        CURLPAUSE_CONT,
        0
    };
    for (int i = 0; i < 5; i++) {
        curl_easy_pause(handle, pause_masks[i]);
    }

    // Perform the transfer
    res = curl_easy_perform(handle);
    (void)res; // Ignore result for fuzzing

    // Fuzz curl_easy_getinfo with different info types
    long http_code;
    double total_time;
    curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &http_code);
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);

    // Fuzz curl_easy_upkeep
    curl_easy_upkeep(handle);

    // Cleanup
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}