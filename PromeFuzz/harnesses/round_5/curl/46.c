// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static CURL *create_test_handle(void) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return NULL;
    }
    
    // Set some basic options to make the handle usable
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    return curl;
}

static void cleanup_test_handle(CURL *curl) {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Initialize libcurl if needed
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }
    
    CURL *curl = create_test_handle();
    if (!curl) {
        return 0;
    }
    
    // Use fuzzer input to create a dummy file if needed
    if (Size > 0) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
        }
    }
    
    // Test curl_easy_perform - this will likely fail but that's OK for fuzzing
    curl_easy_perform(curl);
    
    // Test curl_easy_upkeep
    curl_easy_upkeep(curl);
    
    // Test curl_easy_pause with various bitmasks derived from input
    int bitmask = 0;
    if (Size > 0) {
        bitmask = Data[0] % 4; // CURLPAUSE_RECV=1, CURLPAUSE_SEND=2
        if (bitmask == 0) bitmask = 1; // Ensure at least one bit is set
    } else {
        bitmask = CURLPAUSE_RECV;
    }
    curl_easy_pause(curl, bitmask);
    
    // Test curl_easy_send
    size_t sent = 0;
    if (Size > 0) {
        curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &sent);
    } else {
        char dummy_buffer[] = "test";
        curl_easy_send(curl, dummy_buffer, sizeof(dummy_buffer), &sent);
    }
    
    // Test curl_easy_recv
    size_t received = 0;
    char recv_buffer[1024];
    if (Size > 0) {
        size_t recv_size = Size > sizeof(recv_buffer) ? sizeof(recv_buffer) : Size;
        curl_easy_recv(curl, recv_buffer, recv_size, &received);
    } else {
        curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &received);
    }
    
    // Test curl_ws_recv
    size_t ws_received = 0;
    const struct curl_ws_frame *meta = NULL;
    char ws_buffer[1024];
    if (Size > 0) {
        size_t ws_size = Size > sizeof(ws_buffer) ? sizeof(ws_buffer) : Size;
        curl_ws_recv(curl, ws_buffer, ws_size, &ws_received, &meta);
    } else {
        curl_ws_recv(curl, ws_buffer, sizeof(ws_buffer), &ws_received, &meta);
    }
    
    // Clean up
    cleanup_test_handle(curl);
    
    // Remove dummy file if created
    remove("./dummy_file");
    
    return 0;
}