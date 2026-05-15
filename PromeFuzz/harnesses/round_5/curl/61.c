// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use first byte to determine which path to take
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Always set some basic options
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    // Try different API paths based on selector
    if (selector & 0x01) {
        // Test curl_easy_setopt with various options
        if (fuzz_size > 0) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(fuzz_data[0] & 1));
        }
        if (fuzz_size > 1) {
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(fuzz_data[1] & 1));
        }
    }
    
    // Perform transfer
    CURLcode res = curl_easy_perform(curl);
    
    // Test curl_easy_getinfo after perform
    if (selector & 0x02) {
        double dval;
        long lval;
        char *cval = NULL;
        
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dval);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lval);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &cval);
        
        // Test with various CURLINFO options
        if (fuzz_size > 2) {
            switch (fuzz_data[2] % 4) {
                case 0:
                    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &cval);
                    break;
                case 1:
                    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &dval);
                    break;
                case 2:
                    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &lval);
                    break;
                case 3:
                    curl_easy_getinfo(curl, CURLINFO_SSL_VERIFYRESULT, &lval);
                    break;
            }
        }
    }
    
    // Test WebSocket and send/recv functions (requires CONNECT_ONLY)
    if (selector & 0x04) {
        // Set up for CONNECT_ONLY
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
        curl_easy_setopt(curl, CURLOPT_URL, "ws://dummy");
        
        // Try another perform (will likely fail, but that's OK)
        curl_easy_perform(curl);
        
        // Test curl_ws_send
        if (fuzz_size > 0) {
            size_t sent = 0;
            char buffer[1024];
            size_t buf_len = fuzz_size > sizeof(buffer) ? sizeof(buffer) : fuzz_size;
            if (buf_len > 0) {
                memcpy(buffer, fuzz_data, buf_len);
                unsigned int flags = 0;
                if (fuzz_size > 3 && (fuzz_data[3] & 1)) flags |= CURLWS_TEXT;
                if (fuzz_size > 3 && (fuzz_data[3] & 2)) flags |= CURLWS_BINARY;
                
                curl_ws_send(curl, buffer, buf_len, &sent, 0, flags);
            }
        }
        
        // Test curl_ws_recv
        if (fuzz_size > 1) {
            char recv_buffer[1024];
            size_t recv_len = 0;
            const struct curl_ws_frame *meta = NULL;
            size_t buf_size = fuzz_size > sizeof(recv_buffer) ? sizeof(recv_buffer) : fuzz_size;
            if (buf_size > 0) {
                curl_ws_recv(curl, recv_buffer, buf_size, &recv_len, &meta);
            }
        }
        
        // Test curl_easy_send
        if (fuzz_size > 2) {
            size_t sent = 0;
            char send_buffer[512];
            size_t send_len = fuzz_size > sizeof(send_buffer) ? sizeof(send_buffer) : fuzz_size;
            if (send_len > 0) {
                memcpy(send_buffer, fuzz_data, send_len);
                curl_easy_send(curl, send_buffer, send_len, &sent);
            }
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}