// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_header at headers.c:55:11 in header.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CURL_EXTERN
#define CURL_EXTERN extern
#endif

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/header.h"
#include "curl/websockets.h"

static void dummy_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    (void)size;
    (void)nmemb;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        size_t sent = 0;
        if (Size > 0) {
            curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &sent);
        }
    }
    
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        size_t sent = 0;
        if (Size > 0) {
            unsigned int flags = (Size > 0) ? (Data[0] & 0xFF) : 0;
            curl_off_t fragsize = (Size > 1) ? (Data[1] & 0xFF) : 0;
            curl_ws_send(curl, Data, Size > 1024 ? 1024 : Size, &sent, fragsize, flags);
        }
    }
    
    struct curl_header *header = NULL;
    struct curl_header *prev = NULL;
    
    for (unsigned int origin = 0; origin < 4; origin++) {
        for (int request = 0; request < 2; request++) {
            prev = NULL;
            do {
                header = curl_easy_nextheader(curl, origin, request, prev);
                prev = header;
            } while (header);
        }
    }
    
    if (Size > 0) {
        char *name = malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            
            struct curl_header *hout = NULL;
            for (unsigned int origin = 0; origin < 4; origin++) {
                for (int request = 0; request < 2; request++) {
                    for (size_t index = 0; index < 4; index++) {
                        curl_easy_header(curl, name, index, origin, request, &hout);
                    }
                }
            }
            free(name);
        }
    }
    
    for (int opt = 1; opt < 300; opt++) {
        if (opt == CURLOPT_WRITEFUNCTION || opt == CURLOPT_WRITEDATA) {
            continue;
        }
        
        switch (opt) {
            case CURLOPT_URL:
                curl_easy_setopt(curl, opt, "http://localhost/");
                break;
            case CURLOPT_CONNECT_ONLY:
                curl_easy_setopt(curl, opt, 1L);
                break;
            case CURLOPT_VERBOSE:
                curl_easy_setopt(curl, opt, (Size > 0) ? (long)(Data[0] & 1) : 0L);
                break;
            case CURLOPT_POSTFIELDS:
                if (Size > 0) {
                    curl_easy_setopt(curl, opt, Data);
                }
                break;
            case CURLOPT_POSTFIELDSIZE:
                curl_easy_setopt(curl, opt, (long)(Size > 1024 ? 1024 : Size));
                break;
            default:
                if (opt >= CURLOPTTYPE_LONG && opt < CURLOPTTYPE_OFF_T) {
                    long val = (Size > 0) ? (long)(Data[0] & 0xFF) : 0L;
                    curl_easy_setopt(curl, opt, val);
                }
                break;
        }
    }
    
    curl_easy_cleanup(curl);
    return 0;
}