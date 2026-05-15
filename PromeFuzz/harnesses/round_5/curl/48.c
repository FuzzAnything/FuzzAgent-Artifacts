// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    /* Write fuzzer data to dummy file for potential file operations */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    /* Use first byte to determine which options to set */
    uint8_t option_selector = Data[0];
    size_t data_offset = 1;
    
    /* Set various options based on fuzzer input */
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    
    if (option_selector & 0x01) {
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    }
    
    if (option_selector & 0x02 && data_offset < Size) {
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_callback);
    }
    
    if (option_selector & 0x04 && data_offset < Size) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
    }
    
    /* Perform the transfer */
    CURLcode res = curl_easy_perform(curl);
    
    /* Get various info after transfer */
    if (res == CURLE_OK) {
        double time_value;
        long response_code;
        char *effective_url = NULL;
        
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_value);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        /* Test send/recv if CONNECT_ONLY was set */
        if (option_selector & 0x01) {
            size_t sent = 0, received = 0;
            char buffer[1024];
            
            if (data_offset < Size) {
                size_t send_size = Size - data_offset;
                if (send_size > sizeof(buffer)) send_size = sizeof(buffer);
                curl_easy_send(curl, Data + data_offset, send_size, &sent);
            }
            
            curl_easy_recv(curl, buffer, sizeof(buffer), &received);
        }
    }
    
    /* Get error string if perform failed */
    if (res != CURLE_OK) {
        const char *error_str = curl_easy_strerror(res);
        (void)error_str; /* Use error_str to avoid unused variable warning */
    }
    
    /* Additional getinfo calls with various info types */
    if (option_selector & 0x08) {
        double dbl_val;
        long long_val;
        char *str_val = NULL;
        curl_off_t off_val;
        
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &str_val);
        curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &long_val);
        curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &dbl_val);
    }
    
    /* Clean up */
    curl_easy_cleanup(curl);
    
    /* Remove dummy file */
    remove("./dummy_file");
    
    return 0;
}