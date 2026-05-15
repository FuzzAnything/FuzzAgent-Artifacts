// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/easy.h"

static int dummy_file_initialized = 0;
static const char* dummy_filename = "./dummy_file";

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen(dummy_filename, "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    if (dummy_file_initialized) {
        remove(dummy_filename);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Initialize dummy file with fuzzer data
    write_dummy_file(Data, Size);
    dummy_file_initialized = 1;
    
    // Initialize curl handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Test curl_easy_setopt with various options using fuzzer data
    // Use first byte to select option type
    if (Size > 0) {
        uint8_t opt_type = Data[0] % 6;
        
        switch (opt_type) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_URL, dummy_filename);
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[0] & 1));
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (long)(Data[0] & 1));
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_UPLOAD, (long)(Data[0] & 1));
                break;
            case 4:
                if (Size > 1) {
                    long timeout = (long)Data[1];
                    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
                }
                break;
            case 5:
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, (long)(Data[0] & 1));
                break;
        }
    }
    
    // Perform transfer (may fail, but that's OK for fuzzing)
    curl_easy_perform(curl);
    
    // Test curl_easy_getinfo with various info types
    if (Size > 0) {
        uint8_t info_type = Data[0] % 4;
        long long_val;
        double double_val;
        char *str_val = NULL;
        
        switch (info_type) {
            case 0:
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
                break;
            case 1:
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_val);
                break;
            case 2:
                curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &str_val);
                break;
            case 3:
                curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_val);
                break;
        }
    }
    
    // Test send/recv functions if CONNECT_ONLY was set
    // Use remaining data for buffer operations
    if (Size > 1) {
        size_t buffer_size = Size - 1;
        void *buffer = malloc(buffer_size);
        if (buffer) {
            memcpy(buffer, Data + 1, buffer_size);
            size_t n;
            
            // Test curl_easy_send
            curl_easy_send(curl, buffer, buffer_size, &n);
            
            // Test curl_easy_recv
            curl_easy_recv(curl, buffer, buffer_size, &n);
            
            // Test curl_ws_recv if WebSocket support is available
            const struct curl_ws_frame *meta = NULL;
            curl_ws_recv(curl, buffer, buffer_size, &n, &meta);
            
            free(buffer);
        }
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}