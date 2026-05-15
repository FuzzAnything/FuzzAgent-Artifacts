// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_EXTERN extern
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

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

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLcode res;
    long opt_long;
    char *opt_string;
    curl_off_t opt_offset;
    void *opt_func;
    
    struct curl_ws_frame *meta = NULL;
    size_t recv_len = 0;
    size_t sent_len = 0;
    char buffer[1024];
    double info_double;
    long info_long;
    char *info_string;
    
    int option_type = Data[0] % 6;
    size_t data_offset = 1;
    int connect_only_set = 0;
    
    switch (option_type) {
        case 0:
            opt_long = (Size > data_offset) ? Data[data_offset] : 1L;
            curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_long);
            break;
        case 1:
            opt_string = (Size > data_offset) ? (char *)&Data[data_offset] : "file://./dummy_file";
            curl_easy_setopt(curl, CURLOPT_URL, opt_string);
            break;
        case 2:
            opt_long = 1L;
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, opt_long);
            connect_only_set = 1;
            break;
        case 3:
            opt_long = (Size > data_offset) ? Data[data_offset] : 0L;
            curl_easy_setopt(curl, CURLOPT_NOBODY, opt_long);
            break;
        case 4:
            opt_offset = (Size > data_offset) ? Data[data_offset] : 0;
            curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, opt_offset);
            break;
        case 5:
            opt_func = NULL;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, opt_func);
            break;
    }
    
    res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        if (connect_only_set) {
            size_t buflen = (Size - data_offset < sizeof(buffer)) ? Size - data_offset : sizeof(buffer);
            if (buflen > 0) {
                curl_easy_send(curl, &Data[data_offset], buflen, &sent_len);
                curl_easy_recv(curl, buffer, sizeof(buffer), &recv_len);
                curl_ws_recv(curl, buffer, sizeof(buffer), &recv_len, (const struct curl_ws_frame **)&meta);
            }
        }
        
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &info_double);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info_long);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &info_string);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &info_string);
    }
    
    curl_easy_cleanup(curl);
    remove_dummy_file();
    
    return 0;
}