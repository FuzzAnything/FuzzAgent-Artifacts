// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
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

#ifdef __cplusplus
extern "C" {
#endif

#include "curl/multi.h"
#include "curl/easy.h"

#ifdef __cplusplus
}
#endif

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
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
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    long timeout_val;
    int ret_val;
    int running_handles;
    int msgs_in_queue;
    
    curl_multi_timeout(multi_handle, &timeout_val);
    
    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = 0;
    extra_fds[0].events = (short)(Data[0] & 0x03);
    extra_fds[1].fd = 1;
    extra_fds[1].events = (short)((Data[0] >> 2) & 0x03);
    
    unsigned int extra_nfds = (unsigned int)(Data[0] % 3);
    int timeout_ms = (int)(Data[0] % 1000);
    
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
    
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, 0, &running_handles);
    
    if (Size > 1) {
        int socket_fd = (int)(Data[1] % 1024);
        int ev_bitmask = (int)(Data[1] & 0x07);
        curl_multi_socket_action(multi_handle, (curl_socket_t)socket_fd, ev_bitmask, &running_handles);
    }
    
    curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
    
    curl_multi_perform(multi_handle, &running_handles);
    
    CURLMsg *msg;
    do {
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    } while (msg);
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    remove_dummy_file();
    
    return 0;
}