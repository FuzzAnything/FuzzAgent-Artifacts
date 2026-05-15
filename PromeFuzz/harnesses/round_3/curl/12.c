// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    write_dummy_file(Data, Size);

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        char url[256];
        snprintf(url, sizeof(url), "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        
        CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
        
        int running_handles = 0;
        CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);
        
        int timeout_ms = (int)(Data[0] | (Data[1] << 8));
        int ret = 0;
        
        struct curl_waitfd extra_fds[2];
        unsigned int extra_nfds = Size > 8 ? 2 : 0;
        
        if (extra_nfds > 0) {
            extra_fds[0].fd = (curl_socket_t)(Data[2] | (Data[3] << 8));
            extra_fds[0].events = (short)(Data[4] & 0xFF);
            extra_fds[0].revents = 0;
            
            if (Size > 8) {
                extra_fds[1].fd = (curl_socket_t)(Data[5] | (Data[6] << 8));
                extra_fds[1].events = (short)(Data[7] & 0xFF);
                extra_fds[1].revents = 0;
            }
        }
        
        CURLMcode wait_res = curl_multi_wait(multi_handle, 
                                           extra_nfds > 0 ? extra_fds : NULL,
                                           extra_nfds,
                                           timeout_ms,
                                           &ret);
        
        CURLMcode poll_res = curl_multi_poll(multi_handle,
                                           extra_nfds > 0 ? extra_fds : NULL,
                                           extra_nfds,
                                           timeout_ms,
                                           &ret);
        
        int msgs_in_queue = 0;
        CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        while (msg != NULL) {
            msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        }
        
        curl_socket_t dummy_socket = (curl_socket_t)(Data[0] | (Data[1] << 8));
        int ev_bitmask = (int)(Data[2] & 0xFF);
        CURLMcode socket_res = curl_multi_socket_action(multi_handle,
                                                      dummy_socket,
                                                      ev_bitmask,
                                                      &running_handles);
        
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    cleanup_dummy_file();
    
    return 0;
}