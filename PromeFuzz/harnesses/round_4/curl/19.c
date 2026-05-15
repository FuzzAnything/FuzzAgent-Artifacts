// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
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
#include <unistd.h>
#include <fcntl.h>
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static CURL* create_easy_handle(const uint8_t *data, size_t size) {
    CURL *easy = curl_easy_init();
    if (easy) {
        write_dummy_file(data, size);
        curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
    }
    return easy;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLM *multi = curl_multi_init();
    if (!multi) return 0;
    
    CURL *easy = create_easy_handle(Data, Size);
    if (easy) {
        curl_multi_add_handle(multi, easy);
    }
    
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    switch (selector % 6) {
        case 0: {
            curl_multi_wakeup(multi);
            break;
        }
        case 1: {
            curl_socket_t s = -1;
            if (offset < Size) {
                s = (curl_socket_t)Data[offset];
                offset++;
            }
            int running = 0;
            curl_multi_socket(multi, s, &running);
            break;
        }
        case 2: {
            curl_socket_t s = -1;
            int ev_bitmask = 0;
            if (offset < Size) {
                s = (curl_socket_t)Data[offset];
                offset++;
            }
            if (offset < Size) {
                ev_bitmask = Data[offset];
                offset++;
            }
            int running = 0;
            curl_multi_socket_action(multi, s, ev_bitmask, &running);
            break;
        }
        case 3: {
            CURLMcode code = CURLM_OK;
            if (offset < Size) {
                code = (CURLMcode)(Data[offset] % (CURLM_LAST + 1));
            }
            curl_multi_strerror(code);
            break;
        }
        case 4: {
            if (easy) {
                curl_multi_remove_handle(multi, easy);
            }
            break;
        }
        case 5: {
            int running = 0;
            curl_multi_perform(multi, &running);
            break;
        }
    }
    
    if (easy) {
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi);
    
    unlink("./dummy_file");
    return 0;
}