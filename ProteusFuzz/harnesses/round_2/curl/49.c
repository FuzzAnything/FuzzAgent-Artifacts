// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl library functions:
 * 1. curl_easy_send
 * 2. curl_easy_ssls_import
 * 3. curl_version_info
 *
 * Note: The Wcurl_* macros are compile-time warnings and not callable runtime functions.
 * They are exercised implicitly by the compiler when using curl_easy_setopt/getinfo,
 * but cannot be directly invoked in a fuzz driver.
 */

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * Test 1: curl_version_info
     * This function is safe to call at any time and does not require a valid connection.
     * We fuzz the 'stamp' argument by interpreting the first byte of input.
     */
    if (Size >= 1) {
        CURLversion stamp = (CURLversion)(Data[0] & 0x0F); /* Limit to reasonable range */
        curl_version_info_data *info = curl_version_info(stamp);
        if (info) {
            /* Access fields to ensure they are reachable */
            (void)info->version;
            (void)info->version_num;
            (void)info->host;
            (void)info->features;
            (void)info->ssl_version;
        }
    }

    /*
     * Test 2: curl_easy_ssls_import
     * Requires: handle, session_key, shmac, shmac_len, sdata, sdata_len
     * We use the input data to construct these parameters.
     */
    if (Size >= 8) {
        /* Extract lengths from input */
        size_t shmac_len = (Size > 1) ? (Data[1] % 100) : 0;
        size_t sdata_len = (Size > 2) ? (Data[2] % 100) : 0;

        /* Ensure we have enough data */
        if (Size >= 3 + shmac_len + sdata_len) {
            const char *session_key = (const char *)(Data + 3);
            const unsigned char *shmac = (const unsigned char *)(Data + 3 + shmac_len);
            const unsigned char *sdata = (const unsigned char *)(Data + 3 + shmac_len + sdata_len);

            /* Call curl_easy_ssls_import */
            curl_easy_ssls_import(curl, session_key, shmac, shmac_len, sdata, sdata_len);
        }
    }

    /*
     * Test 3: curl_easy_send
     * Requires: A connected socket (via CURLOPT_CONNECT_ONLY and curl_easy_perform).
     * Since we cannot easily establish a real connection in a fuzzing environment,
     * we will attempt to call curl_easy_send with various inputs to test error handling.
     * This will likely return CURLE_BAD_FUNCTION_ARGUMENT or CURLE_NOT_BUILT_IN,
     * which is expected and useful for fuzzing.
     */
    if (Size >= 4) {
        size_t buflen = (Size > 4) ? (Data[3] % 1000) : 0;
        size_t n = 0;
        const void *buffer = (Size > 4) ? (const void *)(Data + 4) : NULL;

        /* Attempt to send data (expected to fail without a connection) */
        curl_easy_send(curl, buffer, buflen, &n);
    }

    /*
     * Additional exploration: Try setting some options to trigger type-checking macros
     * indirectly, although the macros themselves are compile-time.
     * We set a URL to a dummy value to prepare for a potential connect.
     */
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:12345");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Attempt perform (will likely fail due to no server, but exercises the path) */
    curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}