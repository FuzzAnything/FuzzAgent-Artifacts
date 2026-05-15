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
#include "curl/curl.h"
#include "curl/header.h"
#include "curl/websockets.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Write data to dummy file for potential file-based operations
    write_dummy_file(Data, Size);
    
    // Test curl_easy_setopt with various options
    // Use fuzzer data to select options and values
    int opt_selector = Data[0] % 10;
    switch (opt_selector) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            break;
        case 1:
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            break;
        case 2:
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            break;
        case 3:
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // For WebSocket
            break;
        case 4:
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L); // For recv
            break;
        default:
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost");
            break;
    }
    
    // Test curl_easy_perform (will likely fail but exercise code paths)
    curl_easy_perform(curl);
    
    // Test curl_easy_header and curl_easy_nextheader
    struct curl_header *header = NULL;
    struct curl_header *prev = NULL;
    unsigned int origin = Data[0] % 3;
    int request = Data[0] % 2;
    
    // Try to get a header
    CURLHcode hcode = curl_easy_header(curl, "test-header", 0, origin, request, &header);
    
    // Iterate through headers if available
    if (hcode == CURLHE_OK && header) {
        while (header) {
            prev = header;
            header = curl_easy_nextheader(curl, origin, request, prev);
        }
    }
    
    // Test curl_easy_recv
    size_t n = 0;
    char recv_buffer[1024];
    if (Size > 0 && Size < sizeof(recv_buffer)) {
        curl_easy_recv(curl, recv_buffer, Size, &n);
    }
    
    // Test curl_ws_send
    size_t sent = 0;
    curl_off_t fragsize = (curl_off_t)(Data[0] % 256);
    unsigned int flags = Data[0] % 8;
    
    if (Size > 0) {
        // Use fuzzer data as buffer
        char *ws_buffer = (char*)malloc(Size);
        if (ws_buffer) {
            memcpy(ws_buffer, Data, Size);
            curl_ws_send(curl, ws_buffer, Size, &sent, fragsize, flags);
            free(ws_buffer);
        }
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    
    return 0;
}