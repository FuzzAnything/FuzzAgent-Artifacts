// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "multi.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_files(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_files();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    long timeout_val = 0;
    int ret_val = 0;
    curl_off_t off_val = 0;
    unsigned int fd_count = 0;
    
    unsigned char selector = Data[0];
    
    switch (selector % 6) {
        case 0: {
            CURLMinfo_offt info_type = (CURLMinfo_offt)(selector % 4);
            curl_multi_get_offt(multi_handle, info_type, &off_val);
            break;
        }
        case 1:
            curl_multi_timeout(multi_handle, &timeout_val);
            break;
        case 2: {
            unsigned int size = (selector % 32);
            struct curl_waitfd *ufds = NULL;
            if (size > 0) {
                ufds = (struct curl_waitfd *)malloc(size * sizeof(struct curl_waitfd));
                if (ufds) {
                    for (unsigned int i = 0; i < size; i++) {
                        ufds[i].fd = -1;
                        ufds[i].events = (short)(selector % 4);
                        ufds[i].revents = 0;
                    }
                }
            }
            curl_multi_waitfds(multi_handle, ufds, size, &fd_count);
            free(ufds);
            break;
        }
        case 3: {
            int timeout_ms = (int)(selector % 1000);
            curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret_val);
            break;
        }
        case 4:
            if (easy_handle) {
                curl_multi_remove_handle(multi_handle, easy_handle);
                curl_multi_add_handle(multi_handle, easy_handle);
            }
            break;
        case 5:
            curl_multi_perform(multi_handle, &running_handles);
            break;
    }
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    cleanup_dummy_files();
    
    return 0;
}