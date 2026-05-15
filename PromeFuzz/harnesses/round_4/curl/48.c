// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
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
#include <sys/select.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
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
    curl_multi_timeout(multi_handle, &timeout_val);
    
    curl_socket_t dummy_socket = (curl_socket_t)(Data[0] & 0x7F);
    void *sockp = (void *)(uintptr_t)Data[0];
    curl_multi_assign(multi_handle, dummy_socket, sockp);
    
    struct curl_waitfd extra_fds[2];
    int num_fds = (Data[0] % 3);
    for (int i = 0; i < num_fds && i < 2; i++) {
        extra_fds[i].fd = (curl_socket_t)(Data[i] & 0x7F);
        extra_fds[i].events = (short)(Data[i] % 8);
        extra_fds[i].revents = 0;
    }
    
    int wait_ret, poll_ret;
    int timeout_ms = (Data[0] % 1000);
    curl_multi_wait(multi_handle, extra_fds, num_fds, timeout_ms, &wait_ret);
    curl_multi_poll(multi_handle, extra_fds, num_fds, timeout_ms, &poll_ret);
    
    fd_set read_fds, write_fds, exc_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exc_fds);
    int max_fd = 0;
    curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
    
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    remove_dummy_file();
    
    return 0;
}