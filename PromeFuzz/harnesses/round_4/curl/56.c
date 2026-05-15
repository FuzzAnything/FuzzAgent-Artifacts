// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Define CURL_DISABLE_TYPECHECK to avoid macro expansion issues */
#define CURL_DISABLE_TYPECHECK
#include "multi.h"

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

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    curl_socket_t dummy_sock = (curl_socket_t)(Data[0] & 0x7F);
    void *dummy_ptr = (void *)(Data + 1);
    int ev_bitmask = (int)(Data[0] & 0x07);
    
    switch (Data[0] % 6) {
        case 0:
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        case 1:
            curl_multi_assign(multi_handle, dummy_sock, dummy_ptr);
            break;
        case 2:
            curl_multi_socket(multi_handle, dummy_sock, &running_handles);
            break;
        case 3:
            curl_multi_socket_action(multi_handle, dummy_sock, ev_bitmask, &running_handles);
            break;
        case 4:
            if (Size > 1) {
                switch (Data[1] % 4) {
                    case 0:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
                        break;
                    case 1:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, dummy_ptr);
                        break;
                    case 2:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
                        break;
                    case 3:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, dummy_ptr);
                        break;
                }
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
    cleanup_dummy_file();
    
    return 0;
}