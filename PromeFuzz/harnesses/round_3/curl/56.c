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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static size_t dummy_write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test curl_easy_setopt with various options
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    // Test curl_easy_perform
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Test curl_easy_strerror
    const char *error_str = curl_easy_strerror(perform_res);
    
    // Test curl_easy_getinfo with various info types
    long http_code = 0;
    double total_time = 0.0;
    const char *content_type = NULL;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    
    // Test curl_easy_send and curl_easy_recv when in CONNECT_ONLY mode
    if (perform_res == CURLE_OK) {
        size_t sent = 0;
        size_t received = 0;
        char buffer[1024];
        
        // Use a portion of fuzzer data for send/recv
        size_t send_size = Size > sizeof(buffer) ? sizeof(buffer) : Size;
        
        curl_easy_send(curl, Data, send_size, &sent);
        curl_easy_recv(curl, buffer, sizeof(buffer), &received);
    }
    
    // Additional curl_easy_setopt tests with fuzzer data
    if (Size > 10) {
        // Test with string option
        char url_copy[256];
        size_t copy_size = Size > sizeof(url_copy) - 1 ? sizeof(url_copy) - 1 : Size;
        memcpy(url_copy, Data, copy_size);
        url_copy[copy_size] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url_copy);
        
        // Test with long option
        long timeout = (Data[0] << 8) | Data[1];
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    
    return 0;
}