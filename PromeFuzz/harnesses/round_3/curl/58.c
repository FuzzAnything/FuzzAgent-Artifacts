// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

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

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    
    switch (selector) {
        case 0:
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 1:
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        case 2:
            if (Size - data_offset >= sizeof(curl_socket_t)) {
                memcpy(&dummy_socket, Data + data_offset, sizeof(curl_socket_t));
                data_offset += sizeof(curl_socket_t);
            }
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            break;
        case 3:
            if (Size - data_offset >= sizeof(curl_socket_t) + sizeof(int)) {
                memcpy(&dummy_socket, Data + data_offset, sizeof(curl_socket_t));
                data_offset += sizeof(curl_socket_t);
                int ev_bitmask = 0;
                memcpy(&ev_bitmask, Data + data_offset, sizeof(int));
                curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            }
            break;
        case 4:
            if (Size - data_offset >= sizeof(CURLMoption) + sizeof(long)) {
                CURLMoption option;
                memcpy(&option, Data + data_offset, sizeof(CURLMoption));
                data_offset += sizeof(CURLMoption);
                long param = 0;
                memcpy(&param, Data + data_offset, sizeof(long));
                curl_multi_setopt(multi_handle, option, param);
            }
            break;
        case 5:
            if (Size - data_offset >= sizeof(curl_socket_t)) {
                memcpy(&dummy_socket, Data + data_offset, sizeof(curl_socket_t));
                data_offset += sizeof(curl_socket_t);
                void *sockp = (void *)(Data + data_offset);
                curl_multi_assign(multi_handle, dummy_socket, sockp);
            }
            break;
    }
    
    curl_multi_cleanup(multi_handle);
    cleanup_dummy_file();
    
    return 0;
}