// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_handles(CURLM *multi_handle, CURL **easy_handles, int num_easy) {
    if (multi_handle) {
        for (int i = 0; i < num_easy; i++) {
            if (easy_handles[i]) {
                curl_multi_remove_handle(multi_handle, easy_handles[i]);
                curl_easy_cleanup(easy_handles[i]);
            }
        }
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    int num_easy = (Data[0] % 3) + 1;
    CURL **easy_handles = malloc(num_easy * sizeof(CURL *));
    if (!easy_handles) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    for (int i = 0; i < num_easy; i++) {
        easy_handles[i] = curl_easy_init();
        if (easy_handles[i]) {
            char url[256];
            snprintf(url, sizeof(url), "file://./dummy_file?param=%d", i);
            curl_easy_setopt(easy_handles[i], CURLOPT_URL, url);
            curl_easy_setopt(easy_handles[i], CURLOPT_TIMEOUT_MS, 100L);
        }
    }
    
    int operation = Data[0] % 10;
    size_t data_offset = 1;
    
    switch (operation) {
        case 0:
        case 1:
        case 2:
            for (int i = 0; i < num_easy; i++) {
                if (easy_handles[i]) {
                    curl_multi_add_handle(multi_handle, easy_handles[i]);
                }
            }
            break;
            
        case 3:
        case 4:
            for (int i = 0; i < num_easy; i++) {
                if (easy_handles[i]) {
                    curl_multi_add_handle(multi_handle, easy_handles[i]);
                }
            }
            for (int i = 0; i < num_easy; i++) {
                if (easy_handles[i]) {
                    curl_multi_remove_handle(multi_handle, easy_handles[i]);
                }
            }
            break;
            
        case 5:
        case 6: {
            for (int i = 0; i < num_easy; i++) {
                if (easy_handles[i]) {
                    curl_multi_add_handle(multi_handle, easy_handles[i]);
                }
            }
            CURL **handles_array = curl_multi_get_handles(multi_handle);
            if (handles_array) {
                free(handles_array);
            }
            break;
        }
            
        case 7:
        case 8: {
            struct curl_waitfd extra_fds[2];
            int timeout_ms = (Data[data_offset % Size] % 1000) + 1;
            int ret = 0;
            
            extra_fds[0].fd = 0;
            extra_fds[0].events = (Data[data_offset % Size] & 1) ? CURL_WAIT_POLLIN : CURL_WAIT_POLLOUT;
            extra_fds[1].fd = 1;
            extra_fds[1].events = (Data[(data_offset + 1) % Size] & 1) ? CURL_WAIT_POLLIN : CURL_WAIT_POLLOUT;
            
            curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret);
            
            int running_handles = 0;
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
            
        case 9: {
            for (int i = 0; i < num_easy; i++) {
                if (easy_handles[i]) {
                    curl_multi_add_handle(multi_handle, easy_handles[i]);
                }
            }
            
            int running_handles = 0;
            for (int iter = 0; iter < 3; iter++) {
                curl_multi_perform(multi_handle, &running_handles);
                
                struct curl_waitfd extra_fds[1];
                int timeout_ms = 10;
                int ret = 0;
                curl_multi_wait(multi_handle, extra_fds, 0, timeout_ms, &ret);
            }
            break;
        }
    }
    
    cleanup_handles(multi_handle, easy_handles, num_easy);
    free(easy_handles);
    
    return 0;
}