// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
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

    // Create a CURL handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to avoid immediate failures in some paths
    // Using a simple string from input if possible, otherwise a default
    if (Size > 0) {
        char *url = malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            // Ensure it looks somewhat like a URL for the fuzzer to explore states
            if (url[0] != 'h' && url[0] != 'w') {
                url[0] = 'h';
            }
            curl_easy_setopt(curl, CURLOPT_URL, url);
            free(url);
        }
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    // Set connect only to allow websocket operations without full transfer
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    // Perform the connection
    res = curl_easy_perform(curl);
    // We don't fail here, we want to test subsequent functions even if connection failed

    // Test curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res;

    // Test curl_ws_start_frame
    // Flags and length derived from input
    unsigned int flags = (Size > 0) ? (unsigned int)(Data[0] & 0xFF) : 0;
    
    // Fixed bounds check: need at least 3 bytes for Data[1] and Data[2]
    curl_off_t frame_len = 0;
    if (Size >= 3) {
        frame_len = (curl_off_t)((Data[1] << 8) | Data[2]);
    }
    
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    // Test curl_ws_send
    size_t sent = 0;
    
    // Fixed bounds check: need at least 4 bytes for Data[3]
    curl_off_t fragsize = 0;
    if (Size >= 4) {
        fragsize = (curl_off_t)Data[3];
    }

    unsigned int send_flags = 0;
    if (Size >= 5) {
        send_flags = (unsigned int)(Data[4] & 0xFF);
    }
    
    // Send a small portion of the input data
    size_t buflen = 0;
    if (Size >= 6) {
        buflen = (size_t)Data[5];
    }
    
    // Ensure buflen is within valid range
    if (buflen > Size) buflen = Size;
    if (buflen == 0 && Size > 0) buflen = 1; // Ensure non-zero for testing if data exists

    if (Size > 0 && buflen > 0) {
        res = curl_ws_send(curl, Data, buflen, &sent, fragsize, send_flags);
        (void)res;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}