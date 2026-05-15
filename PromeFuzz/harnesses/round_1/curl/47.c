// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_recv at easy.c:1224:10 in easy.h
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
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_47(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    CURLcode res;
    const char *err_str;
    size_t bytes_processed;
    double dval;
    long lval;
    const char *cstr;
    
    write_dummy_file(data, size);
    
    unsigned char selector = data[0];
    size_t offset = 1;
    
    switch (selector % 6) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
            if (size > offset) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)(size - offset));
                curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, data + offset);
            }
            break;
            
        case 1:
            curl_easy_setopt(curl, CURLOPT_URL, "./dummy_file");
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            break;
            
        case 2:
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            break;
            
        case 3:
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            break;
            
        case 4:
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            if (size > offset) {
                curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)(size - offset));
            }
            break;
            
        case 5:
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            break;
    }
    
    res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        err_str = curl_easy_strerror(res);
    }
    
    if (selector % 3 == 0) {
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dval);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lval);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &cstr);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &cstr);
    }
    
    if (selector % 2 == 0 && res == CURLE_OK) {
        char buffer[1024];
        curl_easy_recv(curl, buffer, sizeof(buffer), &bytes_processed);
        
        if (size > offset && bytes_processed > 0) {
            size_t send_size = size - offset;
            if (send_size > sizeof(buffer)) send_size = sizeof(buffer);
            curl_easy_send(curl, data + offset, send_size, &bytes_processed);
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}