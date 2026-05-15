// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static int dummy_notify_callback(CURLM *multi, int timeout, void *userp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, 
                                 void *userp, void *socketp) {
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static char dummy_file[] = "./dummy_file";
    
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    if (Size < 1) {
        return 0;
    }
    
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    int fd = open(dummy_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > offset) {
            size_t write_size = Size - offset;
            if (write_size > 1024) write_size = 1024;
            write(fd, Data + offset, write_size);
            offset += write_size;
        }
        close(fd);
        
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    curl_socket_t dummy_socket = 0;
    unsigned int notification = 0;
    CURLMoption option = 0;
    long param = 0;
    
    if (Size > offset) {
        memcpy(&dummy_socket, Data + offset, sizeof(dummy_socket));
        offset += sizeof(dummy_socket);
    }
    
    if (Size > offset) {
        memcpy(&notification, Data + offset, sizeof(notification));
        offset += sizeof(notification);
    }
    
    if (Size > offset) {
        memcpy(&option, Data + offset, sizeof(option));
        offset += sizeof(option);
    }
    
    if (Size > offset) {
        memcpy(&param, Data + offset, sizeof(param));
        offset += sizeof(param);
    }
    
    switch (selector % 6) {
        case 0:
            curl_multi_socket_action(multi_handle, dummy_socket, 
                                    (int)(notification & 0xFF), &running_handles);
            break;
        case 1:
            curl_multi_notify_enable(multi_handle, notification);
            break;
        case 2:
            switch (option % 10) {
                case 0:
                    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                    break;
                case 1:
                    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, NULL);
                    break;
                case 2:
                    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                    break;
                case 3:
                    curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, NULL);
                    break;
                case 4:
                    curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
                    break;
                case 5:
                    curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYDATA, NULL);
                    break;
                case 6:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, param);
                    break;
                case 7:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, param);
                    break;
                case 8:
                    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, param);
                    break;
                case 9:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, param);
                    break;
            }
            break;
        case 3:
            curl_multi_notify_disable(multi_handle, notification);
            break;
        case 4:
            curl_multi_remove_handle(multi_handle, easy_handle);
            break;
        case 5:
            curl_multi_wakeup(multi_handle);
            break;
    }
    
    if (fd >= 0) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    unlink(dummy_file);
    
    return 0;
}