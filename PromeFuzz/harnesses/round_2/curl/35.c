// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static size_t dummy_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    return size * nmemb;
}

static size_t formget_callback(void *arg, const char *buf, size_t len) {
    return len;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    curl_mime *mime = curl_mime_init(curl);
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            char name_buf[32] = {0};
            char content_buf[32] = {0};
            size_t name_len = (Size > 1) ? (Data[1] % 31) + 1 : 10;
            size_t content_len = (Size > 2) ? (Data[2] % 31) + 1 : 10;
            
            if (name_len > 31) name_len = 31;
            if (content_len > 31) content_len = 31;
            
            for (size_t i = 0; i < name_len && i + 3 < Size; i++) {
                name_buf[i] = Data[i + 3];
            }
            for (size_t i = 0; i < content_len && i + name_len + 3 < Size; i++) {
                content_buf[i] = Data[i + name_len + 3];
            }
            
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, name_buf,
                        CURLFORM_COPYCONTENTS, content_buf,
                        CURLFORM_END);
            break;
        }
        case 1: {
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "field1",
                        CURLFORM_FILE, "./dummy_file",
                        CURLFORM_END);
            break;
        }
        case 2: {
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "field2",
                        CURLFORM_BUFFER, "buffer_name",
                        CURLFORM_BUFFERPTR, Data,
                        CURLFORM_BUFFERLENGTH, (Size > 1024) ? 1024 : Size,
                        CURLFORM_END);
            break;
        }
        default:
            break;
    }
    
    if (formpost) {
        char dummy_arg[16];
        curl_formget(formpost, dummy_arg, formget_callback);
        curl_formfree(formpost);
    }
    
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            curl_mime_data(part, (const char *)Data, (Size > 1024) ? 1024 : Size);
            curl_mime_name(part, "mime_field");
            curl_mime_filename(part, "./dummy_file");
        }
        curl_mime_free(mime);
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/test");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
    
    if (Size > 10) {
        long opt_val = (long)Data[1];
        curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_val & 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (opt_val >> 1) & 1);
    }
    
    curl_easy_cleanup(curl);
    
    return 0;
}