// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "multi.h"
#include "easy.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    write_dummy_file(data, size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    curl_multi_add_handle(multi_handle, easy_handle);
    
    int running_handles = 0;
    
    int selector = data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: {
            curl_socket_t sock = -1;
            int ev_bitmask = 0;
            
            if (offset + sizeof(curl_socket_t) <= size) {
                memcpy(&sock, data + offset, sizeof(curl_socket_t));
                offset += sizeof(curl_socket_t);
            }
            if (offset + sizeof(int) <= size) {
                memcpy(&ev_bitmask, data + offset, sizeof(int));
                offset += sizeof(int);
            }
            
            CURLMcode result = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
            const char *error_str = curl_multi_strerror(result);
            (void)error_str;
            break;
        }
        
        case 1: {
            CURLMcode error_code = CURLM_OK;
            if (offset < size) {
                error_code = (CURLMcode)(data[offset] % (CURLM_LAST + 1));
            }
            const char *error_str = curl_multi_strerror(error_code);
            (void)error_str;
            break;
        }
        
        case 2: {
            long param = 0;
            
            if (offset + sizeof(long) <= size) {
                memcpy(&param, data + offset, sizeof(long));
            }
            
            CURLMcode result = CURLM_OK;
            
            int opt_selector = 0;
            if (offset < size) {
                opt_selector = data[offset] % 5;
            }
            
            switch (opt_selector) {
                case 0:
                    result = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, param);
                    break;
                case 1:
                    result = curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, param);
                    break;
                case 2:
                    result = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, param);
                    break;
                case 3:
                    result = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, param);
                    break;
                case 4:
                    result = curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, param);
                    break;
            }
            
            const char *error_str = curl_multi_strerror(result);
            (void)error_str;
            break;
        }
        
        case 3: {
            CURLMcode result = curl_multi_remove_handle(multi_handle, easy_handle);
            const char *error_str = curl_multi_strerror(result);
            (void)error_str;
            
            if (result == CURLM_OK) {
                curl_multi_add_handle(multi_handle, easy_handle);
            }
            break;
        }
        
        case 4: {
            CURLMcode result = curl_multi_wakeup(multi_handle);
            const char *error_str = curl_multi_strerror(result);
            (void)error_str;
            break;
        }
        
        case 5: {
            curl_socket_t sock = -1;
            
            if (offset + sizeof(curl_socket_t) <= size) {
                memcpy(&sock, data + offset, sizeof(curl_socket_t));
            }
            
            CURLMcode result = curl_multi_socket(multi_handle, sock, &running_handles);
            const char *error_str = curl_multi_strerror(result);
            (void)error_str;
            break;
        }
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    unlink("./dummy_file");
    return 0;
}