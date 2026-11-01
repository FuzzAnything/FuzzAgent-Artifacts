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
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Initialize libcurl globally */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare a dummy URL to ensure connection context exists */
    const char *url = "http://localhost:9999";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOPROXY, "*");

    /* Use input data to drive flags and lengths */
    unsigned int flags = (Data[0] % 10) + 1;
    curl_off_t frame_len = (Size > 2) ? ((curl_off_t)Data[1] << 8 | Data[2]) : 0;
    size_t buflen = (Size > 3) ? Data[3] : 1;
    curl_off_t fragsize = (Size > 4) ? Data[4] : 0;

    /* Ensure buffers are within bounds */
    if (buflen > Size) {
        buflen = Size;
    }

    /* Attempt to perform connection */
    curl_easy_perform(curl);

    /* Fuzz curl_ws_start_frame */
    curl_ws_start_frame(curl, flags, frame_len);

    /* Fuzz curl_ws_send */
    size_t sent = 0;
    const void *buffer = (Size > 0) ? (const void *)Data : (const void *)"";
    if (buflen > 0 && buffer) {
        curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
    }

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}