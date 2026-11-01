// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* Dummy callbacks to satisfy type checking macros if needed */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    CURLMcode res;
    const char *err_str;

    if (Size < 4) {
        return 0;
    }

    /* Initialize libcurl global environment */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Set a dummy URL to avoid immediate errors in some paths */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
        
        /* Add easy handle to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* Interpret input data to drive diverse states */
    uint32_t action_type = 0;
    if (Size >= 4) {
        memcpy(&action_type, Data, 4);
    }

    /* Fuzz curl_multi_assign */
    /* We use a dummy socket descriptor derived from input */
    if (Size >= 8) {
        int sock_val;
        memcpy(&sock_val, Data + 4, sizeof(int));
        sockfd = (curl_socket_t)sock_val;
        
        /* Use a portion of the input as the custom pointer */
        if (Size > 8) {
            sockp = (void *)(Data + 8);
        }
        
        res = curl_multi_assign(multi_handle, sockfd, sockp);
        (void)res; /* Ignore return for fuzzing stability */
    }

    /* Fuzz curl_multi_perform */
    if (action_type % 4 == 0) {
        res = curl_multi_perform(multi_handle, &running_handles);
        (void)res;
    }

    /* Fuzz curl_multi_socket (deprecated) */
    if (action_type % 4 == 1) {
        res = curl_multi_socket(multi_handle, sockfd, &running_handles);
        (void)res;
    }

    /* Fuzz curl_multi_socket_all (deprecated) */
    if (action_type % 4 == 2) {
        res = curl_multi_socket_all(multi_handle, &running_handles);
        (void)res;
    }

    /* Fuzz curl_multi_strerror with various error codes */
    if (Size >= 12) {
        CURLMcode error_code;
        memcpy(&error_code, Data + 8, sizeof(CURLMcode));
        err_str = curl_multi_strerror(error_code);
        if (err_str) {
            /* Consume the string to avoid unused variable warnings */
            (void)strlen(err_str);
        }
    }

    /* Cleanup */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    curl_global_cleanup();

    return 0;
}