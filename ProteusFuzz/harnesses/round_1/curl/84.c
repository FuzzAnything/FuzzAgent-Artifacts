// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Prepare environment: Set basic options to avoid immediate errors
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Interpret input data to drive fuzzing
    uint32_t seed = 0;
    if (Size >= 4) {
        memcpy(&seed, Data, sizeof(uint32_t));
    }

    int running_handles = 0;
    curl_socket_t sock = -1;
    void *sockp = NULL;

    // Fuzz curl_multi_perform
    if (seed & 1) {
        curl_multi_perform(multi_handle, &running_handles);
    }

    // Fuzz curl_multi_socket (Deprecated but available)
    if (seed & 2) {
        // Use a dummy socket value derived from seed
        sock = (curl_socket_t)(seed % 1024);
        curl_multi_socket(multi_handle, sock, &running_handles);
    }

    // Fuzz curl_multi_socket_all (Deprecated but available)
    if (seed & 4) {
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    // Fuzz curl_multi_assign
    if (seed & 8) {
        sock = (curl_socket_t)(seed % 1024);
        // Create a dummy pointer based on seed to avoid NULL issues if needed, 
        // though NULL is often valid for the 'sockp' argument depending on usage.
        sockp = (void *)((uintptr_t)(seed & 0xFFFF));
        curl_multi_assign(multi_handle, sock, sockp);
    }

    // Fuzz curl_easy_setopt with various types to trigger type-checking macros
    // We use a subset of options to ensure diversity without crashing.
    // The macros provided in typecheck-gcc.h are compile-time checks, 
    // but passing invalid runtime data can still trigger internal logic.
    
    if (Size > 4) {
        // Create a string buffer from input for string options
        char *str_buf = malloc(Size - 4 + 1);
        if (str_buf) {
            memcpy(str_buf, Data + 4, Size - 4);
            str_buf[Size - 4] = '\0';

            // Try setting URL (String option)
            curl_easy_setopt(easy_handle, CURLOPT_URL, str_buf);
            
            // Try setting POSTFIELDS (Postfields option)
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, str_buf);

            // Try setting a long option with a value derived from seed
            curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)(seed % 1000));

            free(str_buf);
        }
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}