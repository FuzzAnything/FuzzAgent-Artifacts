// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
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
#include "curl/curl.h"
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    curl_multi_add_handle(multi_handle, easy_handle);
    
    int running_handles = 0;
    curl_socket_t dummy_socket = (curl_socket_t)(Data[0] % 256);
    int ev_bitmask = (int)(Data[0] % 8);
    
    for (size_t i = 0; i < Size && i < 10; i++) {
        CURLMcode code = (CURLMcode)(Data[i] % (CURLM_LAST + 1));
        
        const char *error_str = curl_multi_strerror(code);
        (void)error_str;
        
        long opt_val = (long)Data[i];
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, opt_val);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, opt_val);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, opt_val);
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, opt_val % 3);
        
        if (i % 2 == 0) {
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
        }
        
        if (i % 3 == 0) {
            curl_multi_remove_handle(multi_handle, easy_handle);
            if (i % 6 == 0) {
                curl_multi_add_handle(multi_handle, easy_handle);
            }
        }
        
        if (i % 4 == 0) {
            curl_multi_wakeup(multi_handle);
        }
        
        if (i % 5 == 0) {
            curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
        }
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    unlink("./dummy_file");
    return 0;
}