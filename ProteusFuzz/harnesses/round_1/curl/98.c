// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    size_t sent = 0;
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    curl_off_t fragsize = 0;
    
    /* Initialize libcurl globally */
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Set basic options to make the handle valid */
    /* Use a dummy URL to avoid actual network connections */
    const char *dummy_url = "http://localhost:0";
    curl_easy_setopt(curl, CURLOPT_URL, dummy_url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    /* Perform the connection (might fail, which is acceptable for fuzzing) */
    res = curl_easy_perform(curl);

    /* Try to use WebSocket functions */
    /* Parse flags and length from input data if available */
    if (Size >= 1) {
        flags = Data[0];
    }
    if (Size >= 8) {
        frame_len = *(const curl_off_t *)(Data + 1);
    }

    /* Start a WebSocket frame */
    res = curl_ws_start_frame(curl, flags, frame_len);

    /* Try to send data */
    if (Size >= 9) {
        fragsize = *(const curl_off_t *)(Data + 1);
        size_t buflen = Size - 1;
        const void *buffer = Data + 1;
        res = curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
    }

    /* Perform connection upkeep */
    res = curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}