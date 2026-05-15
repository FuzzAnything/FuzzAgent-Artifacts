// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_free_all at slist.c:124:6 in curl.h
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
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    size_t bytes_processed = 0;
    char *buffer = NULL;
    struct curl_ws_frame *meta = NULL;
    const struct curl_ws_frame **metap = (const struct curl_ws_frame **)&meta;
    double double_info = 0.0;
    long long_info = 0;
    char *string_info = NULL;
    struct curl_slist *slist = NULL;
    
    // Initialize curl
    curl = curl_easy_init();
    if (!curl) {
        goto cleanup;
    }
    
    // Use first byte to determine which functions to test
    uint8_t selector = Size > 0 ? Data[0] : 0;
    size_t data_offset = Size > 0 ? 1 : 0;
    size_t data_size = Size > 0 ? Size - 1 : 0;
    
    // Always set some basic options
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    // Test curl_easy_setopt with various parameter types
    if (data_size > 0) {
        // Test with long parameter
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(selector & 1));
        
        // Test with string parameter
        char *dummy_str = "dummy";
        curl_easy_setopt(curl, CURLOPT_USERAGENT, dummy_str);
        
        // Test with function pointer
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dummy_callback);
        
        // Test with object pointer
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl);
    }
    
    // Perform the transfer (will likely fail without network, but that's OK)
    res = curl_easy_perform(curl);
    
    // Test curl_easy_getinfo with different info types
    if (selector & 0x01) {
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_info);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &string_info);
    }
    
    // Test send/recv functions if we have data
    if (data_size > 0) {
        // Allocate buffer for send/recv operations
        buffer = (char *)malloc(data_size);
        if (buffer) {
            // Copy fuzzing data to buffer
            memcpy(buffer, Data + data_offset, data_size);
            
            // Test curl_easy_send
            if (selector & 0x02) {
                curl_easy_send(curl, buffer, data_size, &bytes_processed);
            }
            
            // Test curl_easy_recv
            if (selector & 0x04) {
                curl_easy_recv(curl, buffer, data_size, &bytes_processed);
            }
            
            // Test curl_ws_recv
            if (selector & 0x08) {
                curl_ws_recv(curl, buffer, data_size, &bytes_processed, metap);
            }
            
            free(buffer);
        }
    }
    
    // Test with file operations if needed
    if (selector & 0x10) {
        FILE *dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            if (data_size > 0) {
                fwrite(Data + data_offset, 1, data_size, dummy_file);
            }
            fclose(dummy_file);
            
            // Set file option
            dummy_file = fopen("./dummy_file", "rb");
            if (dummy_file) {
                curl_easy_setopt(curl, CURLOPT_READDATA, dummy_file);
                fclose(dummy_file);
            }
        }
    }
    
cleanup:
    if (slist) {
        curl_slist_free_all(slist);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    
    return 0;
}