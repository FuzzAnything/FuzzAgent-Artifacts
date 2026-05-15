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
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include "/promefuzz/database/curl/latest/code/include/curl/easy.h"

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    setup_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Test curl_easy_setopt with various options
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    // Test curl_easy_perform
    CURLcode perform_result = curl_easy_perform(curl);
    
    // Test curl_easy_strerror with the result
    const char *error_str = curl_easy_strerror(perform_result);
    (void)error_str; // Use variable to avoid unused warning
    
    // Test curl_easy_getinfo with various info types
    long response_code;
    double total_time;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    
    // Test curl_easy_send if connected
    if (perform_result == CURLE_OK) {
        size_t sent = 0;
        char send_buffer[256];
        size_t send_size = Size > sizeof(send_buffer) ? sizeof(send_buffer) : Size;
        memcpy(send_buffer, Data, send_size);
        curl_easy_send(curl, send_buffer, send_size, &sent);
        
        // Test curl_easy_recv
        char recv_buffer[256];
        size_t received = 0;
        curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &received);
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}