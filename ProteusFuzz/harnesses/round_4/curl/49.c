// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
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

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    size_t sent;
    curl_off_t fragsize;
    unsigned int flags;
    curl_off_t frame_len;
    long init_flags;

    if (Size < 24) {
        return 0;
    }

    // Extract parameters from input data safely
    init_flags = *(long *)(Data);
    fragsize = *(curl_off_t *)(Data + sizeof(long));
    flags = *(unsigned int *)(Data + sizeof(long) + sizeof(curl_off_t));
    frame_len = *(curl_off_t *)(Data + sizeof(long) + sizeof(curl_off_t) + sizeof(unsigned int));

    // Test curl_global_init
    res = curl_global_init(init_flags);
    if (res != CURLE_OK) {
        // Initialization failed, try to proceed anyway or return
    }

    // Test curl_global_init_mem with custom callbacks (using standard malloc/free as fallback)
    res = curl_global_init_mem(init_flags, malloc, free, realloc, strdup, calloc);
    if (res != CURLE_OK) {
        // Initialization with custom memory failed
    }

    // Create a CURL handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to avoid immediate errors
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    // Test curl_easy_perform
    res = curl_easy_perform(curl);
    // Ignore result, just exercise the function

    // Test curl_ws_send
    // Prepare a buffer from input data
    size_t buflen = Size > 100 ? 100 : Size;
    res = curl_ws_send(curl, Data, buflen, &sent, fragsize, flags);
    // Ignore result

    // Test curl_ws_start_frame
    res = curl_ws_start_frame(curl, flags, frame_len);
    // Ignore result

    // Test curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    // Ignore result

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}