// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void dummy_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Write data to dummy file for potential file-based options
    dummy_write(Data, Size > 1024 ? 1024 : Size);
    
    // Use first byte as selector for different code paths
    uint8_t selector = Data[0];
    
    // Set various options based on fuzzer input
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    // Try different options based on input
    if (selector & 0x01) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    }
    
    if (selector & 0x02) {
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    }
    
    // Attempt to perform (will likely fail without real connection)
    CURLcode res = curl_easy_perform(curl);
    
    // Try various functions regardless of perform result
    char buffer[1024];
    size_t n = 0;
    
    // Try curl_easy_recv
    if (selector & 0x04) {
        curl_easy_recv(curl, buffer, sizeof(buffer), &n);
    }
    
    // Try curl_easy_send
    if (selector & 0x08) {
        size_t send_size = Size > sizeof(buffer) ? sizeof(buffer) : Size;
        memcpy(buffer, Data, send_size);
        curl_easy_send(curl, buffer, send_size, &n);
    }
    
    // Try curl_ws_recv
    if (selector & 0x10) {
        const struct curl_ws_frame *meta = NULL;
        curl_ws_recv(curl, buffer, sizeof(buffer), &n, &meta);
    }
    
    // Try curl_easy_getinfo with different info types
    long longval = 0;
    double doubleval = 0.0;
    char *strval = NULL;
    
    if (selector & 0x20) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &longval);
    }
    
    if (selector & 0x40) {
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &doubleval);
    }
    
    if (selector & 0x80) {
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &strval);
    }
    
    // Set more options based on remaining data
    if (Size > 1) {
        // Use second byte for more option variations
        uint8_t opt_selector = Data[1];
        
        if (opt_selector & 0x01) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        }
        
        if (opt_selector & 0x02) {
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        }
        
        if (opt_selector & 0x04 && Size > 10) {
            // Use some fuzzer data as a string option
            char tmp_str[256];
            size_t copy_len = Size - 2 > 255 ? 255 : Size - 2;
            memcpy(tmp_str, Data + 2, copy_len);
            tmp_str[copy_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, tmp_str);
        }
    }
    
    // Try perform again with potentially different options
    if (selector & 0x01) {
        curl_easy_perform(curl);
    }
    
    curl_easy_cleanup(curl);
    
    return 0;
}