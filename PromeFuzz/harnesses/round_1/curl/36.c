// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        remove_dummy_file();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        remove_dummy_file();
        return 0;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    
    int running_handles = 0;
    int ret_val = 0;
    struct curl_waitfd extra_fds[2];
    
    unsigned int cmd = Data[0] % 6;
    size_t offset = 1;
    
    switch (cmd) {
        case 0:
            curl_multi_add_handle(multi_handle, easy_handle);
            break;
        case 1: {
            CURL **handles = curl_multi_get_handles(multi_handle);
            if (handles) free(handles);
            break;
        }
        case 2:
            if (offset < Size) {
                curl_multi_add_handle(multi_handle, easy_handle);
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        case 3:
            if (Size - offset >= sizeof(extra_fds[0]) * 2) {
                for (int i = 0; i < 2; i++) {
                    extra_fds[i].fd = *(int*)(Data + offset);
                    offset += sizeof(int);
                    extra_fds[i].events = *(short*)(Data + offset);
                    offset += sizeof(short);
                    extra_fds[i].revents = 0;
                }
                int timeout_ms = (Size > offset) ? (int)Data[offset] : 0;
                curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret_val);
            }
            break;
        case 4:
            curl_multi_add_handle(multi_handle, easy_handle);
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 5:
            curl_multi_perform(multi_handle, &running_handles);
            break;
    }
    
    if (cmd != 2 && cmd != 4) {
        curl_multi_add_handle(multi_handle, easy_handle);
        curl_multi_perform(multi_handle, &running_handles);
        
        if (Size > 1 && (Data[1] % 2)) {
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
    }
    
    CURL **all_handles = curl_multi_get_handles(multi_handle);
    if (all_handles) free(all_handles);
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    remove_dummy_file();
    return 0;
}