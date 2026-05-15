// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include "/promefuzz/database/curl/latest/code/include/curl/easy.h"

static size_t dummy_formget_callback(void *arg, const char *buf, size_t len) {
    return len;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    unsigned char selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (fuzz_size > 0) {
            fwrite(fuzz_data, 1, fuzz_size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) return 0;
    
    switch (selector) {
        case 0: {
            curl_mime* mime = curl_mime_init(easy_handle);
            curl_mime_free(mime);
            curl_mime_free(NULL);
            break;
        }
        
        case 1: {
            struct curl_httppost* formpost = NULL;
            struct curl_httppost* lastptr = NULL;
            
            if (fuzz_size > 0) {
                char* name = malloc(32);
                char* content = malloc(32);
                if (name && content) {
                    strncpy(name, "fuzzname", 31);
                    strncpy(content, "fuzzcontent", 31);
                    name[31] = '\0';
                    content[31] = '\0';
                    
                    curl_formadd(&formpost, &lastptr,
                                CURLFORM_COPYNAME, name,
                                CURLFORM_COPYCONTENTS, content,
                                CURLFORM_END);
                }
                free(name);
                free(content);
            }
            
            if (formpost) {
                curl_formget(formpost, NULL, dummy_formget_callback);
                curl_formfree(formpost);
            }
            break;
        }
        
        case 2: {
            curl_mime* mime = curl_mime_init(easy_handle);
            if (mime) {
                curl_mime_free(mime);
            }
            break;
        }
        
        case 3: {
            struct curl_httppost* formpost = NULL;
            struct curl_httppost* lastptr = NULL;
            
            if (fuzz_size > 0) {
                char* name = malloc(32);
                char* content = malloc(32);
                if (name && content) {
                    strncpy(name, "test", 31);
                    strncpy(content, "data", 31);
                    name[31] = '\0';
                    content[31] = '\0';
                    
                    curl_formadd(&formpost, &lastptr,
                                CURLFORM_COPYNAME, name,
                                CURLFORM_COPYCONTENTS, content,
                                CURLFORM_END);
                }
                free(name);
                free(content);
            }
            
            curl_formfree(formpost);
            curl_formfree(NULL);
            break;
        }
        
        case 4: {
            if (fuzz_size > 0) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
                curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
                
                if (fuzz_size > 1) {
                    long opt_val = (long)(fuzz_data[0] % 2);
                    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, opt_val);
                }
                
                if (fuzz_size > 2) {
                    long timeout = (long)(fuzz_data[1] % 10);
                    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
                }
            }
            break;
        }
        
        case 5: {
            struct curl_httppost* formpost = NULL;
            struct curl_httppost* lastptr = NULL;
            
            if (fuzz_size > 0) {
                CURLFORMcode res = curl_formadd(&formpost, &lastptr,
                                               CURLFORM_COPYNAME, "field1",
                                               CURLFORM_COPYCONTENTS, "value1",
                                               CURLFORM_END);
                
                if (res == CURL_FORMADD_OK && fuzz_size > 10) {
                    char field_name[16];
                    char field_value[16];
                    snprintf(field_name, sizeof(field_name), "field%c", fuzz_data[0] % 26 + 'a');
                    snprintf(field_value, sizeof(field_value), "value%c", fuzz_data[1] % 26 + 'a');
                    
                    curl_formadd(&formpost, &lastptr,
                                CURLFORM_COPYNAME, field_name,
                                CURLFORM_COPYCONTENTS, field_value,
                                CURLFORM_END);
                }
                
                if (formpost) {
                    curl_formfree(formpost);
                }
            }
            break;
        }
    }
    
    curl_easy_cleanup(easy_handle);
    return 0;
}