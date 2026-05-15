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

/* Define CURL_DISABLE_TYPECHECK to avoid typecheck-gcc.h issues */
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    int running_handles = 0;
    CURLMcode res;
    
    /* Use first byte to determine which function to test */
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            /* Test curl_multi_perform */
            res = curl_multi_perform(multi_handle, &running_handles);
            break;
        }
        case 1: {
            /* Test curl_multi_socket_all (deprecated) */
            res = curl_multi_socket_all(multi_handle, &running_handles);
            break;
        }
        case 2: {
            /* Test curl_multi_socket (deprecated) */
            curl_socket_t sock = (curl_socket_t)(Data[1 % Size] + 1000);
            res = curl_multi_socket(multi_handle, sock, &running_handles);
            break;
        }
        case 3: {
            /* Test curl_multi_socket_action */
            curl_socket_t sock = (curl_socket_t)(Data[1 % Size] + 1000);
            int ev_bitmask = (int)(Data[2 % Size] & 0x3);
            res = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
            break;
        }
        case 4: {
            /* Test curl_multi_setopt */
            /* Use a simple option that doesn't require complex arguments */
            res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)0);
            break;
        }
        case 5: {
            /* Test curl_multi_assign */
            curl_socket_t sock = (curl_socket_t)(Data[1 % Size] + 1000);
            void *sockp = (void *)(uintptr_t)Data[2 % Size];
            res = curl_multi_assign(multi_handle, sock, sockp);
            break;
        }
        default:
            res = CURLM_OK;
            break;
    }
    
    /* Clean up */
    curl_multi_cleanup(multi_handle);
    
    /* File operations for additional state exploration */
    if (Size > 10) {
        write_dummy_file(Data, Size);
        cleanup_file();
    }
    
    return 0;
}