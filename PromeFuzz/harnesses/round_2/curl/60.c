// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzer data to dummy file for potential use
    write_dummy_file(Data, Size);
    
    // Test curl_easy_strerror with various CURLcode values
    // Use first byte to select from a range of common error codes
    CURLcode err_code = (CURLcode)(Data[0] % 100); // CURLcode values typically within 0-99
    const char *err_str = curl_easy_strerror(err_code);
    (void)err_str; // Use result to avoid unused variable warning
    
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (curl) {
        // Test curl_easy_setopt with various options using fuzzer data
        // Use different bytes to determine which options to set
        size_t opt_index = Data[0] % 6;
        
        switch (opt_index) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
                break;
            case 4:
                curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
                break;
            case 5:
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                break;
        }
        
        // Test curl_easy_perform (may fail depending on setup)
        CURLcode perform_res = curl_easy_perform(curl);
        
        // Test curl_easy_getinfo with various info types
        long response_code;
        double total_time;
        char *effective_url = NULL;
        
        if (perform_res == CURLE_OK || perform_res == CURLE_UNSUPPORTED_PROTOCOL) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        }
        
        // Test curl_easy_send and curl_easy_recv if CONNECT_ONLY was set
        if (opt_index == 3) {
            size_t sent = 0;
            size_t received = 0;
            char send_buf[128];
            char recv_buf[128];
            
            // Copy some fuzzer data to send buffer
            size_t send_size = Size > sizeof(send_buf) ? sizeof(send_buf) : Size;
            memcpy(send_buf, Data, send_size);
            
            curl_easy_send(curl, send_buf, send_size, &sent);
            curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &received);
        }
        
        // Clean up
        curl_easy_cleanup(curl);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    // Remove dummy file
    remove_dummy_file();
    
    return 0;
}