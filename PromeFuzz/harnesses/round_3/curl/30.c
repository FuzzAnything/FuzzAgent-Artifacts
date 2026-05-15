// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include <sys/select.h>
#include <unistd.h>

static void init_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            const char *default_data = "dummy data";
            fwrite(default_data, 1, strlen(default_data), f);
        }
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

static CURLM *create_multi_handle(void) {
    CURLM *multi = curl_multi_init();
    if (multi) {
        CURL *easy = curl_easy_init();
        if (easy) {
            curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
            curl_multi_add_handle(multi, easy);
            curl_easy_cleanup(easy);
        }
    }
    return multi;
}

static void destroy_multi_handle(CURLM *multi) {
    if (multi) {
        struct CURLMsg *msg;
        int msgs_left;
        while ((msg = curl_multi_info_read(multi, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                CURL *easy = msg->easy_handle;
                curl_multi_remove_handle(multi, easy);
            }
        }
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    init_dummy_file(Data, Size);
    
    CURLM *multi = create_multi_handle();
    if (!multi) {
        cleanup_dummy_file();
        return 0;
    }
    
    unsigned char selector = Data[0];
    size_t data_offset = 1;
    
    if (selector & 0x01) {
        long timeout;
        CURLMcode rc = curl_multi_timeout(multi, &timeout);
        (void)rc;
    }
    
    if (selector & 0x02) {
        struct curl_waitfd extra_fds[2];
        int ret;
        
        extra_fds[0].fd = 0;
        extra_fds[0].events = (short)(data_offset < Size ? Data[data_offset] : 0);
        data_offset = (data_offset + 1) % Size;
        
        extra_fds[1].fd = 1;
        extra_fds[1].events = (short)(data_offset < Size ? Data[data_offset] : 0);
        data_offset = (data_offset + 1) % Size;
        
        int timeout_ms = (data_offset + 2 < Size) ? 
            (int)(Data[data_offset] | (Data[data_offset+1] << 8) | (Data[data_offset+2] << 16)) % 10000 : 1000;
        
        CURLMcode rc = curl_multi_wait(multi, extra_fds, 2, timeout_ms, &ret);
        (void)rc;
    }
    
    if (selector & 0x04) {
        int running_handles;
        CURLMcode rc = curl_multi_perform(multi, &running_handles);
        (void)rc;
        
        if (running_handles > 0) {
            int dummy;
            curl_multi_perform(multi, &dummy);
        }
    }
    
    if (selector & 0x08) {
        struct curl_waitfd extra_fds[1];
        int ret;
        
        extra_fds[0].fd = 2;
        extra_fds[0].events = (short)(data_offset < Size ? Data[data_offset] : 0);
        data_offset = (data_offset + 1) % Size;
        
        int timeout_ms = (data_offset + 1 < Size) ? 
            (int)(Data[data_offset] | (Data[data_offset+1] << 8)) % 5000 : 500;
        
        CURLMcode rc = curl_multi_poll(multi, extra_fds, 1, timeout_ms, &ret);
        (void)rc;
    }
    
    if (selector & 0x10) {
        fd_set read_fds, write_fds, exc_fds;
        int max_fd;
        
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&exc_fds);
        
        CURLMcode rc = curl_multi_fdset(multi, &read_fds, &write_fds, &exc_fds, &max_fd);
        (void)rc;
    }
    
    if (selector & 0x20) {
        curl_socket_t sockfd = (data_offset < Size) ? (curl_socket_t)Data[data_offset] : 0;
        data_offset = (data_offset + 1) % Size;
        
        void *sockp = (data_offset + 3 < Size) ? (void *)(Data + data_offset) : NULL;
        
        CURLMcode rc = curl_multi_assign(multi, sockfd, sockp);
        (void)rc;
    }
    
    destroy_multi_handle(multi);
    cleanup_dummy_file();
    
    return 0;
}