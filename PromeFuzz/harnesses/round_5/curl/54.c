// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Use fuzzer data to determine which operations to perform
    uint8_t operation = Size > 0 ? Data[0] % 6 : 0;
    size_t offset = Size > 1 ? Data[1] % Size : 0;
    size_t usable_size = Size > offset ? Size - offset : 0;

    // Test curl_easy_setopt with various options
    CURLcode res;
    if (usable_size > 0) {
        // Set URL from fuzzer data (truncated to reasonable length)
        char url[256];
        size_t url_len = usable_size < 255 ? usable_size : 255;
        memcpy(url, Data + offset, url_len);
        url[url_len] = '\0';
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
        
        // Try different options based on fuzzer input
        switch (operation) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // For WebSocket
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L); // For recv
                break;
            case 4:
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
                break;
            case 5:
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, stdout);
                break;
        }
    }

    // Perform transfer
    res = curl_easy_perform(curl);
    
    // Test curl_easy_getinfo with various info types
    if (res == CURLE_OK || operation % 2 == 0) {
        double dval;
        long lval;
        char *cval;
        
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dval);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lval);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &cval);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &cval);
    }

    // Test curl_easy_recv if we have buffer space
    if (operation == 3 && usable_size > 0) {
        char recv_buf[1024];
        size_t nrecv = 0;
        size_t buflen = usable_size < 1024 ? usable_size : 1024;
        curl_easy_recv(curl, recv_buf, buflen, &nrecv);
    }

    // Test curl_ws_send if we have data
    if (operation == 2 && usable_size > 0) {
        size_t sent = 0;
        curl_off_t fragsize = usable_size < 100 ? usable_size : 100;
        unsigned int flags = CURLWS_TEXT;
        
        if (usable_size > 1) {
            flags = Data[offset] % 2 ? CURLWS_BINARY : CURLWS_TEXT;
        }
        
        curl_ws_send(curl, Data + offset, usable_size, &sent, fragsize, flags);
    }

    // Test curl_easy_strerror
    const char *err_str = curl_easy_strerror(res);
    (void)err_str; // Use variable to avoid unused warning

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}