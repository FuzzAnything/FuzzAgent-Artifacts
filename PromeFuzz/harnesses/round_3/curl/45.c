// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static int dummy_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    /* Dummy callback to satisfy curl_multi_socket_action requirements */
    return 0;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle;
    CURL *easy_handle;
    int running_handles;
    curl_socket_t dummy_socket;
    CURLMcode res;
    
    /* Initialize multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    /* Initialize easy handle */
    easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Set a dummy URL */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        
        /* Add easy handle to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    /* Create a dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    /* Use the first byte to determine which function to test */
    if (Size > 0) {
        uint8_t selector = Data[0] % 5;
        
        switch (selector) {
            case 0:
                /* Test curl_multi_perform */
                res = curl_multi_perform(multi_handle, &running_handles);
                break;
                
            case 1:
                /* Test curl_multi_socket_all (deprecated) */
                res = curl_multi_socket_all(multi_handle, &running_handles);
                break;
                
            case 2:
                /* Test curl_multi_socket (deprecated) */
                dummy_socket = (Size > 1) ? (curl_socket_t)Data[1] : 0;
                res = curl_multi_socket(multi_handle, dummy_socket, &running_handles);
                break;
                
            case 3:
                /* Test curl_multi_socket_action */
                dummy_socket = (Size > 1) ? (curl_socket_t)Data[1] : 0;
                {
                    int ev_bitmask = (Size > 2) ? (int)Data[2] : 0;
                    res = curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
                }
                break;
                
            case 4:
                /* Test curl_multi_assign */
                dummy_socket = (Size > 1) ? (curl_socket_t)Data[1] : 0;
                res = curl_multi_assign(multi_handle, dummy_socket, NULL);
                break;
                
            default:
                res = CURLM_OK;
                break;
        }
    }
    
    /* Clean up */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    
    return 0;
}