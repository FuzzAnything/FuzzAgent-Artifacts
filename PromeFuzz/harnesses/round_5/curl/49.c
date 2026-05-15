// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    unsigned char selector = Data[0];
    size_t data_offset = 1;
    
    for (int i = 0; i < 3 && data_offset < Size; i++) {
        CURLoption option;
        long long_param = 0;
        const char *str_param = NULL;
        void *ptr_param = NULL;
        
        switch (selector % 7) {
            case 0:
                option = CURLOPT_VERBOSE;
                long_param = (data_offset < Size) ? (Data[data_offset] & 1) : 0;
                curl_easy_setopt(curl, option, long_param);
                data_offset++;
                break;
            case 1:
                option = CURLOPT_URL;
                str_param = "file://./dummy_file";
                curl_easy_setopt(curl, option, str_param);
                break;
            case 2:
                option = CURLOPT_CONNECT_ONLY;
                long_param = (data_offset < Size) ? (Data[data_offset] & 1) : 0;
                curl_easy_setopt(curl, option, long_param);
                data_offset++;
                break;
            case 3:
                option = CURLOPT_NOBODY;
                long_param = (data_offset < Size) ? (Data[data_offset] & 1) : 0;
                curl_easy_setopt(curl, option, long_param);
                data_offset++;
                break;
            case 4:
                option = CURLOPT_HEADER;
                long_param = (data_offset < Size) ? (Data[data_offset] & 1) : 0;
                curl_easy_setopt(curl, option, long_param);
                data_offset++;
                break;
            case 5:
                option = CURLOPT_WRITEFUNCTION;
                ptr_param = NULL;
                curl_easy_setopt(curl, option, ptr_param);
                break;
            case 6:
                option = CURLOPT_WRITEDATA;
                ptr_param = NULL;
                curl_easy_setopt(curl, option, ptr_param);
                break;
        }
        selector = (data_offset < Size) ? Data[data_offset] : selector;
    }
    
    CURLcode perform_res = curl_easy_perform(curl);
    
    if (perform_res == CURLE_OK || perform_res == CURLE_COULDNT_CONNECT) {
        CURLINFO info_type;
        long long_val = 0;
        double double_val = 0.0;
        char *str_val = NULL;
        
        switch (selector % 5) {
            case 0:
                info_type = CURLINFO_RESPONSE_CODE;
                curl_easy_getinfo(curl, info_type, &long_val);
                break;
            case 1:
                info_type = CURLINFO_TOTAL_TIME;
                curl_easy_getinfo(curl, info_type, &double_val);
                break;
            case 2:
                info_type = CURLINFO_SIZE_DOWNLOAD;
                curl_easy_getinfo(curl, info_type, &double_val);
                break;
            case 3:
                info_type = CURLINFO_CONTENT_TYPE;
                curl_easy_getinfo(curl, info_type, &str_val);
                break;
            case 4:
                info_type = CURLINFO_EFFECTIVE_URL;
                curl_easy_getinfo(curl, info_type, &str_val);
                break;
        }
        
        if (data_offset < Size) {
            size_t send_n = 0;
            size_t recv_n = 0;
            size_t remaining = Size - data_offset;
            size_t buf_size = (remaining > 256) ? 256 : remaining;
            
            char *buffer = (char*)malloc(buf_size);
            if (buffer) {
                memcpy(buffer, Data + data_offset, buf_size);
                
                curl_easy_send(curl, buffer, buf_size, &send_n);
                curl_easy_recv(curl, buffer, buf_size, &recv_n);
                
                free(buffer);
            }
        }
    }
    
    const char *error_str = curl_easy_strerror(perform_res);
    (void)error_str;
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}