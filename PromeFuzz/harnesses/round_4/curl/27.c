// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Test curl_easy_setopt with various options
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    
    // Test curl_easy_perform
    CURLcode res = curl_easy_perform(curl);
    
    // Test curl_easy_header and curl_easy_nextheader
    struct curl_header *hout = NULL;
    struct curl_header *prev = NULL;
    unsigned int origin = CURLH_HEADER;
    int request = 0;
    
    // Try to get headers with different parameters
    for (int i = 0; i < 3; i++) {
        CURLHcode hcode = curl_easy_header(curl, "Content-Type", i, origin, request, &hout);
        if (hcode == CURLHE_OK && hout) {
            // Iterate through subsequent headers
            prev = hout;
            while ((prev = curl_easy_nextheader(curl, origin, request, prev)) != NULL) {
                // Just iterate through headers
            }
        }
    }
    
    // Test curl_easy_recv
    char recv_buf[1024];
    size_t n = 0;
    if (res == CURLE_OK) {
        // Try to receive data (will likely fail since not CONNECT_ONLY)
        curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n);
    }
    
    // Test curl_ws_send
    size_t sent = 0;
    curl_off_t fragsize = 0;
    unsigned int flags = CURLWS_TEXT;
    
    // Create a buffer from fuzzer data for sending
    size_t send_size = Size > 1024 ? 1024 : Size;
    char *send_buf = malloc(send_size);
    if (send_buf) {
        memcpy(send_buf, Data, send_size);
        curl_ws_send(curl, send_buf, send_size, &sent, fragsize, flags);
        free(send_buf);
    }
    
    // Test with CONNECT_ONLY option
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    curl_easy_perform(curl);  // Will fail but we want to test the state
    
    // Try recv and ws_send again in CONNECT_ONLY state
    curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n);
    
    if (send_buf) {
        char *send_buf2 = malloc(send_size);
        if (send_buf2) {
            memcpy(send_buf2, Data, send_size);
            curl_ws_send(curl, send_buf2, send_size, &sent, fragsize, flags);
            free(send_buf2);
        }
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    
    return 0;
}