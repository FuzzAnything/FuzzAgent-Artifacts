// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzer data to dummy file
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use first byte to determine fuzzing path
    uint8_t path_selector = Data[0];
    
    // Set various options based on fuzzer input
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    // Try performing the transfer
    CURLcode perform_res = curl_easy_perform(curl);
    
    if (perform_res == CURLE_OK) {
        // Test curl_easy_send
        size_t sent = 0;
        if (Size > 1) {
            curl_easy_send(curl, Data + 1, Size - 1, &sent);
        }
        
        // Test curl_easy_recv
        char recv_buf[1024];
        size_t received = 0;
        curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &received);
        
        // Test curl_ws_recv if WebSocket support is available
        if (path_selector & 0x01) {
            const struct curl_ws_frame *meta = NULL;
            size_t ws_received = 0;
            curl_ws_recv(curl, recv_buf, sizeof(recv_buf), &ws_received, &meta);
        }
    }
    
    // Test curl_easy_getinfo with various info types
    double dval = 0.0;
    long lval = 0;
    char *sval = NULL;
    
    // Try different CURLINFO values based on fuzzer input
    switch (path_selector % 8) {
        case 0:
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dval);
            break;
        case 1:
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lval);
            break;
        case 2:
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &sval);
            break;
        case 3:
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &sval);
            break;
        case 4:
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &dval);
            break;
        case 5:
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &dval);
            break;
        case 6:
            curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &lval);
            break;
        case 7:
            curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &dval);
            break;
    }
    
    // Test more curl_easy_setopt variations
    if (Size > 2) {
        // Use second byte for option selection
        uint8_t opt_selector = Data[1];
        
        switch (opt_selector % 6) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(Data[2] % 100));
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, Data[2] & 0x01 ? 1L : 0L);
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, Data[2] & 0x01 ? 1L : 0L);
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_USERAGENT, "fuzzer/1.0");
                break;
            case 4:
                curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)(Data[2] % 10));
                break;
            case 5:
                curl_easy_setopt(curl, CURLOPT_FAILONERROR, Data[2] & 0x01 ? 1L : 0L);
                break;
        }
        
        // Try performing again with new options
        if (opt_selector & 0x80) {
            curl_easy_perform(curl);
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}