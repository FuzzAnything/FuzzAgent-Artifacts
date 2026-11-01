// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl library functions.
 * Targets: curl_easy_getinfo, curl_easy_upkeep, and type-checking macros
 * indirectly via curl_easy_setopt.
 */

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLcode init_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (init_result != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * Use input data to drive fuzzing logic.
     * We interpret parts of the input as option codes, values, and info codes.
     */

    /* Set a dummy URL to make the handle valid for some operations */
    if (curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/dummy") != CURLE_OK) {
        /* Ignore errors, we just want to exercise the code path */
    }

    /*
     * Exercise curl_easy_setopt with various option types derived from input.
     * This indirectly exercises the type-checking macros (Wcurl_*) at compile time.
     * At runtime, we pass diverse values to trigger different internal states.
     */

    if (Size >= 1) {
        long long_val = (long)(Data[0]);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
    }

    if (Size >= 2) {
        curl_off_t off_val = (curl_off_t)(Data[1]);
        curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, off_val);
    }

    if (Size >= 3) {
        char *str_val = (char *)(void *)Data;
        /* Ensure null termination for safety if used as string */
        char safe_str[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(safe_str, Data, len);
        safe_str[len] = '\0';

        curl_easy_setopt(curl, CURLOPT_USERAGENT, safe_str);
        curl_easy_setopt(curl, CURLOPT_REFERER, safe_str);
    }

    /* Exercise curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /*
     * Exercise curl_easy_getinfo with various info types.
     * We use input data to select which info to retrieve.
     */
    if (Size >= 4) {
        int info_code = Data[3];

        long long_out;
        double double_out;
        curl_off_t off_out;
        char *str_out = NULL;
        curl_socket_t socket_out;

        /* Try to get info based on input */
        curl_easy_getinfo(curl, (CURLINFO)info_code, &long_out);
        curl_easy_getinfo(curl, (CURLINFO)info_code, &double_out);
        curl_easy_getinfo(curl, (CURLINFO)info_code, &off_out);
        curl_easy_getinfo(curl, (CURLINFO)info_code, &str_out);
        curl_easy_getinfo(curl, (CURLINFO)info_code, &socket_out);
    }

    /* Clean up */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}