// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static CURLM* multi_handle = NULL;
static CURL* easy_handle = NULL;
static int initialized = 0;

static void cleanup(void) {
    if (multi_handle && easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
        easy_handle = NULL;
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
        multi_handle = NULL;
    }
    initialized = 0;
}

static int init_handles(void) {
    if (initialized) return 1;
    
    multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        multi_handle = NULL;
        return 0;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 1L);
    
    CURLMcode res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        easy_handle = NULL;
        multi_handle = NULL;
        return 0;
    }
    
    initialized = 1;
    return 1;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    if (!init_handles()) return 0;
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    switch (selector % 6) {
        case 0: {
            unsigned int fd_count = 0;
            curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
            
            if (fd_count > 0 && fd_count < 100) {
                struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
                    free(ufds);
                }
            }
            break;
        }
        
        case 1: {
            CURLMinfo_offt info_type = (CURLMinfo_offt)(selector % 4);
            curl_off_t value = 0;
            curl_multi_get_offt(multi_handle, info_type, &value);
            break;
        }
        
        case 2: {
            if (easy_handle && multi_handle) {
                curl_multi_remove_handle(multi_handle, easy_handle);
                curl_multi_add_handle(multi_handle, easy_handle);
            }
            break;
        }
        
        case 3: {
            int timeout_ms = (int)(selector % 256);
            if (data_offset < Size) {
                timeout_ms = (int)Data[data_offset];
                data_offset++;
            }
            
            int ret = 0;
            curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret);
            break;
        }
        
        case 4: {
            long milliseconds = 0;
            curl_multi_timeout(multi_handle, &milliseconds);
            break;
        }
        
        case 5: {
            int running_handles = 0;
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
    }
    
    if (selector % 8 == 0) {
        cleanup();
    }
    
    return 0;
}