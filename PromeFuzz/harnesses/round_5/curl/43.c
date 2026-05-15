// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    // Write fuzzer data to dummy file for potential URL usage
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Set up basic options for the handle
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    // Try performing the transfer (will likely fail without network, but we want to test the functions)
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Test curl_easy_upkeep regardless of perform result
    curl_easy_upkeep(curl);
    
    // Test curl_easy_pause with fuzzer-derived bitmask
    if (Size > 0) {
        int bitmask = Data[0] & (CURLPAUSE_RECV | CURLPAUSE_SEND);
        curl_easy_pause(curl, bitmask);
    }
    
    // Test curl_easy_send with fuzzer data as buffer
    if (Size > 0) {
        size_t sent = 0;
        void *buffer = malloc(Size);
        if (buffer) {
            memcpy(buffer, Data, Size);
            curl_easy_send(curl, buffer, Size, &sent);
            free(buffer);
        }
    }
    
    // Test curl_ws_recv with fuzzer-provided buffer
    if (Size > 0) {
        size_t recv_size = 0;
        const struct curl_ws_frame *meta = NULL;
        void *buffer = malloc(Size);
        if (buffer) {
            curl_ws_recv(curl, buffer, Size, &recv_size, &meta);
            free(buffer);
        }
    }
    
    // Test curl_easy_recv with fuzzer-provided buffer
    if (Size > 0) {
        size_t recv_size = 0;
        void *buffer = malloc(Size);
        if (buffer) {
            curl_easy_recv(curl, buffer, Size, &recv_size);
            free(buffer);
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}