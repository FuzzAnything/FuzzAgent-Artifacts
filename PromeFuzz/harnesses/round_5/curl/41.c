// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include "curl/curl.h"
#include "curl/header.h"
#include "curl/websockets.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    unsigned int opt_selector = Data[0];
    size_t data_offset = 1;
    
    if (data_offset < Size) {
        switch (opt_selector % 10) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)Data[data_offset] & 1);
                break;
            case 2:
                if (Size - data_offset > 0) {
                    char *dummy_str = malloc(Size - data_offset + 1);
                    if (dummy_str) {
                        memcpy(dummy_str, Data + data_offset, Size - data_offset);
                        dummy_str[Size - data_offset] = '\0';
                        curl_easy_setopt(curl, CURLOPT_USERAGENT, dummy_str);
                        free(dummy_str);
                    }
                }
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)Data[data_offset] & 1);
                break;
            case 4:
                curl_easy_setopt(curl, CURLOPT_NOBODY, (long)Data[data_offset] & 1);
                break;
            case 5:
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (long)(Data[data_offset] % 3));
                break;
            default:
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                break;
        }
    }
    
    CURLcode perform_res = curl_easy_perform(curl);
    
    if (perform_res == CURLE_OK || perform_res == CURLE_UNSUPPORTED_PROTOCOL) {
        size_t sent = 0;
        if (Size > 10) {
            curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &sent);
        }
    }
    
    if (opt_selector % 3 == 2) {
        size_t ws_sent = 0;
        unsigned int flags = CURLWS_TEXT;
        if (Size > 20) {
            curl_ws_send(curl, Data + 10, Size > 1034 ? 1024 : Size - 10, 
                        &ws_sent, 0, flags);
        }
    }
    
    struct curl_header *header = NULL;
    struct curl_header *prev = NULL;
    CURLHcode hcode;
    
    for (unsigned int origin = 0; origin < 4; origin++) {
        for (int request = 0; request < 2; request++) {
            hcode = curl_easy_header(curl, "Content-Type", 0, origin, request, &header);
            
            prev = NULL;
            do {
                prev = curl_easy_nextheader(curl, origin, request, prev);
            } while (prev != NULL);
        }
    }
    
    curl_easy_cleanup(curl);
    
    return 0;
}