// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"
#include "curl/easy.h"
#include "curl/header.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Write fuzzer data to dummy file for potential use
    write_dummy_file(Data, Size);
    
    // Test curl_easy_setopt with various options
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // For WebSocket
    
    // Test curl_easy_perform
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Test curl_easy_header and curl_easy_nextheader
    struct curl_header *header = NULL;
    struct curl_header *prev = NULL;
    unsigned int origins[] = {CURLH_HEADER, CURLH_TRAILER, CURLH_CONNECT, CURLH_1XX};
    
    for (size_t i = 0; i < sizeof(origins)/sizeof(origins[0]); i++) {
        // Get first header
        CURLHcode hcode = curl_easy_header(curl, "test", 0, origins[i], 0, &header);
        
        // Iterate through headers
        while (header != NULL) {
            prev = header;
            header = curl_easy_nextheader(curl, origins[i], 0, prev);
        }
    }
    
    // Test curl_easy_recv if perform was successful and CONNECT_ONLY was set
    if (perform_res == CURLE_OK) {
        char recv_buffer[1024];
        size_t n = 0;
        curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &n);
    }
    
    // Test curl_ws_send
    size_t sent = 0;
    unsigned int flags = CURLWS_TEXT;
    if (Size > 0) {
        // Use first byte to determine flags
        flags = Data[0] % 4;
        if (flags == 0) flags = CURLWS_TEXT;
        else if (flags == 1) flags = CURLWS_BINARY;
        else if (flags == 2) flags = CURLWS_CLOSE;
        else flags = CURLWS_PING;
    }
    
    // Prepare buffer for sending (use fuzzer data if available)
    const void *send_buffer = Data;
    size_t send_size = Size > 1024 ? 1024 : Size;
    curl_off_t fragsize = Size > 512 ? 512 : 0;
    
    curl_ws_send(curl, send_buffer, send_size, &sent, fragsize, flags);
    
    // Cleanup
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}