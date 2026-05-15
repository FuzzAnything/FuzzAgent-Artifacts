// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Write fuzz data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Test curl_easy_strerror with various error codes
    if (Size > 0) {
        CURLcode err_code = (CURLcode)(Data[0] % (CURL_LAST + 1));
        const char *err_str = curl_easy_strerror(err_code);
        (void)err_str; // Use result to avoid unused variable warning
    }
    
    // Set various options using fuzz data
    if (Size > 0) {
        // Set URL to dummy file
        curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
        
        // Set write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
        
        // Set various other options based on fuzz data
        for (size_t i = 0; i < Size && i < 10; i++) {
            switch (Data[i] % 8) {
                case 0:
                    curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[i] & 1));
                    break;
                case 1:
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(Data[i] & 1));
                    break;
                case 2:
                    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(Data[i] % 10));
                    break;
                case 3:
                    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (long)(Data[i] & 1));
                    break;
                case 4:
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, (long)(Data[i] & 1));
                    break;
                case 5:
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, (long)(Data[i] % 3));
                    break;
                case 6:
                    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, 
                                   (long)((Data[i] % 4) == 0 ? CURL_HTTP_VERSION_1_0 : 
                                          (Data[i] % 4) == 1 ? CURL_HTTP_VERSION_1_1 :
                                          (Data[i] % 4) == 2 ? CURL_HTTP_VERSION_2_0 :
                                          CURL_HTTP_VERSION_2TLS));
                    break;
                case 7:
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, "fuzz-agent/1.0");
                    break;
            }
        }
    }
    
    // Perform transfer
    CURLcode res = curl_easy_perform(curl);
    
    // Test curl_easy_strerror with the actual result
    const char *perform_err = curl_easy_strerror(res);
    (void)perform_err;
    
    // Test curl_easy_getinfo with various info types
    if (res == CURLE_OK || Size > 0) {
        long response_code;
        double total_time;
        const char *content_type;
        long http_connectcode;
        double speed_download;
        
        // Try different info types
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
        curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &http_connectcode);
        curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);
        
        // Try more info types based on fuzz data
        if (Size > 1) {
            switch (Data[1] % 6) {
                case 0: {
                    curl_off_t upload_size;
                    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_size);
                    break;
                }
                case 1: {
                    curl_off_t download_size;
                    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_size);
                    break;
                }
                case 2: {
                    long ssl_verifyresult;
                    curl_easy_getinfo(curl, CURLINFO_SSL_VERIFYRESULT, &ssl_verifyresult);
                    break;
                }
                case 3: {
                    double namelookup_time;
                    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &namelookup_time);
                    break;
                }
                case 4: {
                    double connect_time;
                    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect_time);
                    break;
                }
                case 5: {
                    long http_version;
                    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &http_version);
                    break;
                }
            }
        }
    }
    
    // Test curl_easy_send if CONNECT_ONLY was set
    if (Size > 2) {
        size_t sent = 0;
        char buffer[256];
        size_t buffer_size = Size > 256 ? 256 : Size;
        
        // Copy some fuzz data to buffer
        for (size_t i = 0; i < buffer_size && i < Size; i++) {
            buffer[i] = Data[i];
        }
        
        CURLcode send_res = curl_easy_send(curl, buffer, buffer_size, &sent);
        (void)send_res;
    }
    
    // Test curl_ws_recv if CONNECT_ONLY was set
    if (Size > 3) {
        char ws_buffer[256];
        size_t recv_len = 0;
        const struct curl_ws_frame *meta = NULL;
        
        CURLcode ws_res = curl_ws_recv(curl, ws_buffer, sizeof(ws_buffer), &recv_len, &meta);
        (void)ws_res;
        
        // If we got metadata, access its fields
        if (meta) {
            int age = meta->age;
            int flags = meta->flags;
            curl_off_t offset = meta->offset;
            curl_off_t bytesleft = meta->bytesleft;
            size_t len = meta->len;
            (void)age; (void)flags; (void)offset; (void)bytesleft; (void)len;
        }
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}