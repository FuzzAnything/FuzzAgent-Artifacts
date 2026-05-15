// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    CURLcode res;
    size_t n;
    char buffer[256];
    double dval;
    long lval;
    char *cval;
    
    write_dummy_file(Data, Size);
    
    unsigned char scenario = Data[0] % 6;
    size_t offset = 1;
    
    switch (scenario) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
            if (Size > offset) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char*)(Data + offset));
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size - offset);
            }
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                curl_easy_strerror(res);
            }
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dval);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lval);
            break;
            
        case 1:
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                curl_easy_recv(curl, buffer, sizeof(buffer), &n);
            }
            curl_easy_strerror(res);
            break;
            
        case 2:
            curl_easy_setopt(curl, CURLOPT_URL, "./dummy_file");
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            if (Size > offset) {
                curl_easy_setopt(curl, CURLOPT_READDATA, fopen("./dummy_file", "rb"));
            }
            res = curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &cval);
            curl_easy_strerror(res);
            break;
            
        case 3:
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            res = curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_FILETIME, &lval);
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &dval);
            break;
            
        case 4:
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            if (Size > offset && Size - offset < 100) {
                char custom_header[256];
                snprintf(custom_header, sizeof(custom_header), "X-Test: %.*s", 
                         (int)(Size - offset), Data + offset);
                struct curl_slist *headers = NULL;
                headers = curl_slist_append(headers, custom_header);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                res = curl_easy_perform(curl);
                curl_slist_free_all(headers);
            } else {
                res = curl_easy_perform(curl);
            }
            curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &lval);
            break;
            
        case 5:
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            res = curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &lval);
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME, &dval);
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &cval);
            curl_easy_strerror(res);
            break;
    }
    
    curl_easy_cleanup(curl);
    return 0;
}