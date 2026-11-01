// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;

    // Initialize libcurl globally
    long flags = 0;
    if (Size > 0) {
        flags = Data[0];
    }
    res = curl_global_init(flags);
    if (res != CURLE_OK) {
        return 0;
    }

    // Create a CURL handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to ensure the handle is somewhat configured
    res = curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:0/");
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Set timeout to avoid hanging
    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    res = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 100L);

    // Set POSTFIELDS if we have enough data, ensuring null termination
    // We must keep the buffer alive for the duration of curl_easy_perform
    char *post_data = NULL;
    if (Size > 10) {
        post_data = malloc(Size);
        if (post_data) {
            memcpy(post_data, Data, Size);
            post_data[Size - 1] = '\0'; // Ensure null termination
            res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
            // Do NOT free post_data here. libcurl may copy it or keep a reference.
            // If libcurl copies it, we can free it after perform.
            // If libcurl keeps a reference, we must NOT free it until cleanup.
            // To be safe and avoid use-after-free, we will free it after perform
            // ONLY IF we are sure libcurl copied it. 
            // However, CURLOPT_POSTFIELDS usually copies the string.
            // But to be absolutely safe against internal logic variations, 
            // we will free it after perform.
        }
    }

    // Fuzz curl_easy_perform
    res = curl_easy_perform(curl);
    (void)res;

    // Safe to free post_data now as curl_easy_perform has finished
    // and libcurl typically copies the POSTFIELDS string.
    if (post_data) {
        free(post_data);
    }

    // Fuzz curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res;

    // Fuzz WebSocket functions if flags suggest it
    unsigned int ws_flags = 0;
    curl_off_t frame_len = 0;
    size_t sent = 0;
    curl_off_t fragsize = 0;

    if (Size >= 4) {
        ws_flags = (unsigned int)Data[1];
        frame_len = (curl_off_t)(Data[2] * 256 + Data[3]);
        fragsize = (curl_off_t)(Size > 5 ? (Data[4] * 256 + Data[5]) : 0);
    }

    // Fuzz curl_ws_start_frame
    res = curl_ws_start_frame(curl, ws_flags, frame_len);
    (void)res;

    // Fuzz curl_ws_send
    // Use a static buffer or the input data directly to avoid allocation issues
    // Ensure we don't pass a buffer that might be freed or invalid
    res = curl_ws_send(curl, Data, Size, &sent, fragsize, ws_flags);
    (void)res;

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}