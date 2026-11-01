// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/* Dummy callback to satisfy type checking if needed, though not directly called in this driver */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Initialize curl global */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to avoid immediate errors on perform if possible, 
       though we are fuzzing the multi interface primarily */
    const char *dummy_url = "http://example.com/fuzz";
    curl_easy_setopt(easy_handle, CURLOPT_URL, dummy_url);
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sock = -1;
    
    /* Interpret input data to drive fuzzing */
    uint32_t action_type = 0;
    if (Size >= 4) {
        memcpy(&action_type, Data, sizeof(uint32_t));
    }

    /* Create a dummy file if needed for file-based options, though not strictly used here */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        if (Size > 4) {
            write(fd, Data + 4, Size - 4);
        }
        close(fd);
    }

    /* Fuzzing logic: Call different multi functions based on input */
    switch (action_type % 4) {
        case 0:
            /* curl_multi_perform */
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 1:
            /* curl_multi_socket_all (Deprecated but valid for testing) */
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        case 2:
            /* curl_multi_socket (Deprecated but valid for testing) */
            /* Use a dummy socket or -1. curl_socket_t is int. */
            curl_socket_t fuzz_sock = -1;
            if (Size >= 8) {
                memcpy(&fuzz_sock, Data + 4, sizeof(curl_socket_t));
            }
            curl_multi_socket(multi_handle, fuzz_sock, &running_handles);
            break;
        case 3:
            /* Try to trigger internal state changes by performing multiple times */
            curl_multi_perform(multi_handle, &running_handles);
            curl_multi_perform(multi_handle, &running_handles);
            break;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}