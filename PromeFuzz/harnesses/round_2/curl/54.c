// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define CURL_DISABLE_TYPECHECK
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include "/promefuzz/database/curl/latest/code/include/curl/multi.h"
#include "/promefuzz/database/curl/latest/code/include/curl/easy.h"

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURL **handle_array = NULL;
    struct curl_waitfd *ufds = NULL;
    unsigned int fd_count = 0;
    int ret_val = 0;
    
    /* Initialize multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    /* Create a dummy easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    /* Use fuzz data to set some easy handle options */
    if (Size > 0) {
        char url[256];
        size_t url_len = Size < 255 ? Size : 255;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        curl_easy_setopt(easy_handle, CURLOPT_PRIVATE, url);
    }
    
    /* Test curl_multi_add_handle */
    curl_multi_add_handle(multi_handle, easy_handle);
    
    /* Test curl_multi_waitfds - first get count */
    curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    /* Allocate and call again if we have fds */
    if (fd_count > 0) {
        ufds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }
    
    /* Test curl_multi_wait with minimal timeout */
    curl_multi_wait(multi_handle, NULL, 0, 0, &ret_val);
    
    /* Test curl_multi_get_handles */
    handle_array = curl_multi_get_handles(multi_handle);
    if (handle_array) {
        free(handle_array);
    }
    
    /* Test curl_multi_remove_handle */
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    /* Add it back for cleanup */
    curl_multi_add_handle(multi_handle, easy_handle);
    
    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    return 0;
}