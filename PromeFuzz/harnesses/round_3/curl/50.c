// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Write fuzzer data to dummy file for potential file-based operations
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Test curl_easy_setopt with various options
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    // Test curl_easy_perform
    CURLcode res = curl_easy_perform(curl);
    
    // Test curl_easy_strerror for error reporting
    const char *error_str = curl_easy_strerror(res);
    (void)error_str; // Suppress unused variable warning
    
    // Test curl_easy_getinfo with various info types
    long response_code;
    double total_time;
    char *effective_url = NULL;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    // Test curl_easy_send and curl_easy_recv if connection was successful
    if (res == CURLE_OK) {
        size_t sent_bytes = 0;
        size_t received_bytes = 0;
        char send_buffer[1024];
        char recv_buffer[1024];
        
        // Use fuzzer data for send buffer if available
        size_t send_size = Size < sizeof(send_buffer) ? Size : sizeof(send_buffer);
        if (send_size > 0) {
            memcpy(send_buffer, Data, send_size);
            curl_easy_send(curl, send_buffer, send_size, &sent_bytes);
        }
        
        // Try to receive data
        curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &received_bytes);
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}