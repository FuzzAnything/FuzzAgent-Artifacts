// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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

static void dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Dummy write callback that does nothing
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    // Dummy read callback that returns 0 (EOF)
    return 0;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Use first few bytes to determine which operations to perform
    size_t index = 0;
    if (Size > 0) {
        uint8_t operation_selector = Data[index++];
        
        // Set some basic options using fuzzer data
        if (Size > index + 1) {
            // Set URL option - use dummy file
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            
            // Set write function
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            
            // Set read function
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            
            // Randomly set some other options based on fuzzer data
            for (int i = 0; i < 5 && index < Size; i++) {
                CURLoption option = (CURLoption)(Data[index++] % 300); // Approximate range
                switch (option) {
                    case CURLOPT_VERBOSE:
                    case CURLOPT_NOPROGRESS:
                    case CURLOPT_NOSIGNAL:
                        curl_easy_setopt(curl, option, 1L);
                        break;
                    case CURLOPT_CONNECT_ONLY:
                        curl_easy_setopt(curl, option, (operation_selector & 0x01) ? 1L : 0L);
                        break;
                    default:
                        // Try to set with long value if we have data
                        if (index < Size) {
                            long val = (long)Data[index++];
                            curl_easy_setopt(curl, option, val);
                        }
                        break;
                }
            }
        }
        
        // Create dummy file for file operations
        FILE *dummy = fopen("./dummy_file", "wb");
        if (dummy) {
            if (Size - index > 0) {
                fwrite(Data + index, 1, Size - index, dummy);
            }
            fclose(dummy);
        }
        
        // Perform the transfer
        CURLcode res = curl_easy_perform(curl);
        
        // If we set CONNECT_ONLY and perform succeeded, try recv/send operations
        if (res == CURLE_OK && (operation_selector & 0x01)) {
            char buffer[1024];
            size_t bytes_transferred = 0;
            const struct curl_ws_frame *meta = NULL;
            
            // Try curl_easy_recv
            if (Size > index + 10) {
                curl_easy_recv(curl, buffer, sizeof(buffer), &bytes_transferred);
            }
            
            // Try curl_easy_send
            if (Size > index + 5) {
                size_t send_size = (Size - index < sizeof(buffer)) ? Size - index : sizeof(buffer);
                curl_easy_send(curl, Data + index, send_size, &bytes_transferred);
            }
            
            // Try curl_ws_recv
            if (Size > index + 15) {
                curl_ws_recv(curl, buffer, sizeof(buffer), &bytes_transferred, &meta);
            }
        }
        
        // Try to get some info after perform
        if (Size > index) {
            double time_val = 0.0;
            long http_code = 0;
            char *effective_url = NULL;
            
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_val);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
            
            // Try a few more info types based on fuzzer data
            for (int i = 0; i < 3 && index < Size; i++) {
                int info_type = (int)(Data[index++] % 100); // Use int instead of CURLINFO
                switch (info_type) {
                    case CURLINFO_CONTENT_TYPE:
                    case CURLINFO_PRIVATE:
                        {
                            char *str = NULL;
                            curl_easy_getinfo(curl, info_type, &str);
                        }
                        break;
                    case CURLINFO_SIZE_DOWNLOAD:
                    case CURLINFO_SIZE_UPLOAD:
                        {
                            double dbl = 0.0;
                            curl_easy_getinfo(curl, info_type, &dbl);
                        }
                        break;
                    default:
                        // Try with double as default
                        {
                            double dbl = 0.0;
                            curl_easy_getinfo(curl, info_type, &dbl);
                        }
                        break;
                }
            }
        }
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}