// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;

    // Initialize libcurl globally
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    // Create a handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Basic setup to prevent immediate crashes on perform
    // Use a dummy URL to avoid network dependency issues
    res = curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1");
    res = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 0L);
    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L);
    res = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    res = curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

    // Fuzzing logic based on input data
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    size_t sent = 0;
    curl_off_t fragsize = 0;

    if (Size > 0) {
        flags = Data[0];
    }
    if (Size > 1) {
        frame_len = Data[1];
    }
    if (Size > 2) {
        fragsize = Data[2];
    }

    // 1. Test curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res;

    // 2. Test curl_ws_start_frame
    // This will likely fail as we don't have a real WS connection, but tests error paths
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    // 3. Test curl_ws_send
    const void *buffer = NULL;
    size_t buflen = 0;

    if (Size > 3) {
        buffer = (const void *)(Data + 3);
        buflen = Size - 3;
    } else {
        static const char dummy[] = "fuzz";
        buffer = dummy;
        buflen = sizeof(dummy) - 1;
    }

    res = curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
    (void)res;

    // 4. Test curl_easy_perform
    res = curl_easy_perform(curl);
    (void)res;

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}