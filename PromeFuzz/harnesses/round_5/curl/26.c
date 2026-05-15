// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
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

#define CURL_DISABLE_TYPECHECK
#include "multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        remove_dummy_file();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    int ret_val = 0;
    long timeout_ms = 0;
    int msgs_in_queue = 0;
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            curl_multi_socket_action(multi_handle, -1, 0, &running_handles);
            break;
        case 1:
            curl_multi_info_read(multi_handle, &msgs_in_queue);
            break;
        case 2:
            curl_multi_timeout(multi_handle, &timeout_ms);
            break;
        case 3:
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 4:
            curl_multi_poll(multi_handle, NULL, 0, (int)(Data[0] % 1000), &ret_val);
            break;
        case 5:
            curl_multi_wait(multi_handle, NULL, 0, (int)(Data[0] % 1000), &ret_val);
            break;
    }
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    remove_dummy_file();
    
    return 0;
}