// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    // Write fuzzer data to dummy file for potential file operations
    write_dummy_file(Data, Size);
    
    // Test curl_easy_setopt with various options
    // Use fuzzer data to determine which options to set
    if (Size > 0) {
        // Set URL based on first byte
        if (Data[0] % 2 == 0) {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
        }
        
        // Set various options based on fuzzer input
        if (Size > 1) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[1] % 2));
        }
        if (Size > 2) {
            curl_easy_setopt(curl, CURLOPT_NOBODY, (long)(Data[2] % 2));
        }
        if (Size > 3) {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (long)(Data[3] % 2));
        }
        
        // Set read function if enough data
        if (Size > 10) {
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        }
    }
    
    // Test curl_easy_perform
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Test curl_easy_getinfo with various info types
    if (Size > 0) {
        double time_val;
        long response_code;
        char *effective_url = NULL;
        
        // Get different info types based on fuzzer input
        switch (Data[0] % 5) {
            case 0:
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_val);
                break;
            case 1:
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                break;
            case 2:
                curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
                break;
            case 3:
                curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &effective_url);
                break;
            case 4:
                curl_easy_getinfo(curl, CURLINFO_PRIVATE, &effective_url);
                break;
        }
    }
    
    // Test curl_easy_send and curl_easy_recv if CONNECT_ONLY was set
    if (Size > 3 && Data[3] % 2 == 1 && perform_res == CURLE_OK) {
        size_t sent = 0;
        size_t received = 0;
        char buffer[1024];
        
        // Try to send some data
        if (Size > 0) {
            size_t to_send = Size > sizeof(buffer) ? sizeof(buffer) : Size;
            curl_easy_send(curl, Data, to_send, &sent);
        }
        
        // Try to receive some data
        curl_easy_recv(curl, buffer, sizeof(buffer), &received);
    }
    
    // Test curl_easy_strerror with various error codes
    if (Size > 0) {
        // Test with perform result
        const char *err_str = curl_easy_strerror(perform_res);
        
        // Test with other possible error codes based on fuzzer data
        if (Size > 1) {
            CURLcode test_code = (CURLcode)(Data[1] % 95); // There are 95 CURLcode values
            curl_easy_strerror(test_code);
        }
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    remove_dummy_file();
    
    return 0;
}