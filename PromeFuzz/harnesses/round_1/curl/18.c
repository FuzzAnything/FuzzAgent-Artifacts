// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Include curl.h first to get CURL_EXTERN definition
#include "curl/curl.h"
// Then include the specific headers
#include "curl/easy.h"
#include "curl/header.h"
#include "curl/websockets.h"

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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use fuzz data to determine which options to set
    uint8_t option_selector = Data[0];
    size_t data_offset = 1;
    
    // Set various options based on fuzz input
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    
    if (Size > data_offset + 4) {
        // Use some bytes to determine option values
        uint32_t opt_val;
        memcpy(&opt_val, Data + data_offset, sizeof(opt_val));
        data_offset += sizeof(opt_val);
        
        switch (option_selector % 10) {
            case 0: curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)opt_val); break;
            case 1: curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long)opt_val); break;
            case 2: curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)opt_val); break;
            case 3: curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, (long)opt_val); break;
            case 4: curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_val & 1 ? 1L : 0L); break;
            case 5: curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, opt_val & 1 ? 1L : 0L); break;
            case 6: curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, opt_val & 1 ? 1L : 0L); break;
            case 7: curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, opt_val & 1 ? 1L : 0L); break;
            case 8: curl_easy_setopt(curl, CURLOPT_NOPROGRESS, opt_val & 1 ? 1L : 0L); break;
            case 9: curl_easy_setopt(curl, CURLOPT_FAILONERROR, opt_val & 1 ? 1L : 0L); break;
        }
    }
    
    // Try to perform the transfer
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Test header functions if we have enough data
    if (Size > data_offset + 8) {
        struct curl_header *header = NULL;
        CURLHcode hcode;
        
        // Try to get a header by name
        const char *header_name = "Content-Type";
        hcode = curl_easy_header(curl, header_name, 0, CURLH_HEADER, 0, &header);
        
        // Try to iterate through headers
        if (header) {
            struct curl_header *next_header = curl_easy_nextheader(curl, CURLH_HEADER, 0, header);
            (void)next_header; // Use to avoid unused warning
        }
        
        // Try different origins and requests
        for (unsigned int origin = 0; origin < 4; origin++) {
            for (int request = 0; request < 2; request++) {
                struct curl_header *h = NULL;
                hcode = curl_easy_header(curl, "Test", 0, origin, request, &h);
                if (h) {
                    struct curl_header *nh = curl_easy_nextheader(curl, origin, request, h);
                    (void)nh;
                }
            }
        }
    }
    
    // Test WebSocket and recv functions with CONNECT_ONLY mode
    if (option_selector & 0x80) {
        CURL *ws_curl = curl_easy_init();
        if (ws_curl) {
            curl_easy_setopt(ws_curl, CURLOPT_URL, "ws://localhost:8080");
            curl_easy_setopt(ws_curl, CURLOPT_CONNECT_ONLY, 2L);
            
            // Try to perform - will likely fail but we want to test the API
            CURLcode ws_perform = curl_easy_perform(ws_curl);
            
            if (ws_perform == CURLE_OK) {
                // Test curl_ws_send
                char buffer[256];
                size_t sent = 0;
                curl_off_t fragsize = 128;
                unsigned int flags = 0;
                
                if (Size > data_offset) {
                    size_t copy_size = Size - data_offset;
                    if (copy_size > sizeof(buffer)) copy_size = sizeof(buffer);
                    memcpy(buffer, Data + data_offset, copy_size);
                    
                    CURLcode send_res = curl_ws_send(ws_curl, buffer, copy_size, 
                                                    &sent, fragsize, flags);
                    (void)send_res;
                }
                
                // Test curl_easy_recv
                char recv_buffer[256];
                size_t n = 0;
                CURLcode recv_res = curl_easy_recv(ws_curl, recv_buffer, 
                                                  sizeof(recv_buffer), &n);
                (void)recv_res;
            }
            
            curl_easy_cleanup(ws_curl);
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}