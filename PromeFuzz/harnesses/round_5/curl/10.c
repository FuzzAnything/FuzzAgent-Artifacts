// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    /* Do nothing */
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    /* Return zero to indicate EOF */
    (void)buffer;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    FILE *dummy_file = NULL;
    
    /* Initialize curl */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        goto cleanup;
    }
    
    /* Write fuzzer data to a dummy file */
    dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        dummy_file = NULL;
    }
    
    /* Set up some basic options for the handle */
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); /* For WebSocket functions */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L); /* Short timeout */
    
    /* Test curl_easy_pause with various bitmasks */
    if (Size > 0) {
        int bitmask = Data[0] % 4; /* 0-3 combinations */
        switch (bitmask) {
            case 0: bitmask = CURLPAUSE_RECV; break;
            case 1: bitmask = CURLPAUSE_SEND; break;
            case 2: bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND; break;
            case 3: bitmask = 0; /* Unpause both */ break;
        }
        curl_easy_pause(curl, bitmask);
    }
    
    /* Test curl_easy_perform (will likely fail due to dummy setup) */
    curl_easy_perform(curl);
    
    /* Test curl_easy_recv */
    if (Size > 0) {
        char recv_buffer[1024];
        size_t n = 0;
        curl_easy_recv(curl, recv_buffer, Size > 1024 ? 1024 : Size, &n);
    }
    
    /* Test curl_easy_send */
    if (Size > 0) {
        size_t n = 0;
        curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &n);
    }
    
    /* Test curl_ws_recv */
    if (Size > 0) {
        char ws_buffer[1024];
        size_t recv = 0;
        const struct curl_ws_frame *meta = NULL;
        curl_ws_recv(curl, ws_buffer, Size > 1024 ? 1024 : Size, &recv, &meta);
    }
    
    /* Test curl_ws_send */
    if (Size > 0) {
        size_t sent = 0;
        curl_off_t fragsize = (Size > 8) ? *(curl_off_t*)Data : 0;
        unsigned int flags = (Size > 12) ? *(unsigned int*)(Data + 8) : 0;
        curl_ws_send(curl, Data, Size > 1024 ? 1024 : Size, &sent, fragsize, flags);
    }

cleanup:
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    remove("./dummy_file");
    return 0;
}