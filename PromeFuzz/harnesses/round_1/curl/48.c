// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "curl/curl.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    for (int i = 0; i < 3 && data_offset < Size; i++) {
        CURLoption option;
        long long_param = 0;
        const char *str_param = NULL;
        size_t param_size = 0;
        
        if (data_offset + sizeof(CURLoption) <= Size) {
            memcpy(&option, Data + data_offset, sizeof(CURLoption));
            data_offset += sizeof(CURLoption);
        } else {
            option = (CURLoption)(selector * i);
        }
        
        if (data_offset + sizeof(long) <= Size) {
            memcpy(&long_param, Data + data_offset, sizeof(long));
            data_offset += sizeof(long);
        } else {
            long_param = selector;
        }
        
        if (data_offset < Size) {
            str_param = (const char *)(Data + data_offset);
            param_size = Size - data_offset;
            data_offset = Size;
        }
        
        switch (selector % 4) {
            case 0:
                curl_easy_setopt(curl, option, long_param);
                break;
            case 1:
                if (str_param) {
                    curl_easy_setopt(curl, option, str_param);
                }
                break;
            case 2:
                curl_easy_setopt(curl, option, NULL);
                break;
            case 3:
                curl_easy_setopt(curl, option, (void *)&long_param);
                break;
        }
    }
    
    if (selector & 0x01) {
        curl_easy_reset(curl);
    }
    
    if (selector & 0x02) {
        curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
        curl_easy_perform(curl);
    }
    
    if (selector & 0x04) {
        CURL *dup_curl = curl_easy_duphandle(curl);
        if (dup_curl) {
            if (selector & 0x08) {
                curl_easy_setopt(dup_curl, CURLOPT_URL, "file://./dummy_file");
                curl_easy_perform(dup_curl);
            }
            curl_easy_cleanup(dup_curl);
        }
    }
    
    if (selector & 0x10) {
        size_t sent = 0;
        const char dummy_buf[] = "test";
        curl_easy_send(curl, dummy_buf, sizeof(dummy_buf) - 1, &sent);
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}