// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Initialize a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Write fuzzer data to dummy file for potential use
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Set up a dummy URL (file protocol) for basic operations
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L); // Short timeout to avoid hangs

    // Fuzz curl_easy_perform
    CURLcode perform_res = curl_easy_perform(curl);

    // Fuzz curl_easy_pause with random bitmask from data
    if (Size > 0) {
        int bitmask = Data[0] % 4; // 0-3 to approximate CURLPAUSE_* combinations
        CURLcode pause_res = curl_easy_pause(curl, bitmask);
        (void)pause_res; // Use result to avoid unused variable warning
    }

    // Setup for CONNECT_ONLY functions (WebSocket and raw socket)
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // Enable WebSocket mode
    // Re-perform to establish connection state (will likely fail, but that's OK for fuzzing)
    CURLcode connect_res = curl_easy_perform(curl);
    (void)connect_res;

    // Fuzz curl_ws_send if we have enough data
    if (Size >= 1) {
        size_t sent = 0;
        curl_off_t fragsize = (Size > 4) ? *(curl_off_t*)Data : 0;
        unsigned int flags = (Size > 8) ? *(unsigned int*)(Data + 4) : 0;
        CURLcode ws_send_res = curl_ws_send(curl, Data, Size, &sent, fragsize, flags);
        (void)ws_send_res;
    }

    // Fuzz curl_easy_recv
    if (Size >= 1) {
        char recv_buf[1024];
        size_t n = 0;
        CURLcode recv_res = curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n);
        (void)recv_res;
    }

    // Fuzz curl_ws_start_frame (requires being in a READFUNCTION callback, but we try anyway)
    if (Size >= 12) {
        unsigned int flags = *(unsigned int*)Data;
        curl_off_t frame_len = *(curl_off_t*)(Data + 4);
        CURLcode start_frame_res = curl_ws_start_frame(curl, flags, frame_len);
        (void)start_frame_res;
    }

    // Fuzz curl_easy_send
    if (Size >= 1) {
        size_t sent = 0;
        CURLcode send_res = curl_easy_send(curl, Data, Size, &sent);
        (void)send_res;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    return 0;
}