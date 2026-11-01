// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback for CURLOPT_WRITEFUNCTION if needed */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)userp;
    return size * nmemb;
}

/* Helper to create a valid easy handle with minimal setup */
static CURL *create_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    }
    return easy;
}

/* Helper to create a valid multi handle */
static CURLM *create_multi_handle(void) {
    return curl_multi_init();
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create handles */
    CURLM *multi = create_multi_handle();
    if (!multi) {
        return 0;
    }

    CURL *easy = create_easy_handle();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    /* Add easy handle to multi handle to make it valid for multi operations */
    curl_multi_add_handle(multi, easy);

    /* Extract data for fuzzing safely */
    unsigned int op = Data[0] % 6; /* Select operation */
    
    /* Ensure bounds before accessing Data array */
    curl_socket_t sockfd = 0;
    void *sockp = NULL;
    CURLMoption option = 0;
    long long_val = 0;
    const char *str_val = "dummy";

    if (Size > 1) {
        sockfd = (curl_socket_t)(Data[1]);
    }
    if (Size > 2) {
        sockfd = (curl_socket_t)(Data[1] * 256 + Data[2]);
    }
    
    if (Size > 3) {
        sockp = (void *)(uintptr_t)(Data[3]);
    }
    if (Size > 4) {
        sockp = (void *)(uintptr_t)(Data[3] * 256 + Data[4]);
    }

    if (Size > 5) {
        option = (CURLMoption)(Data[5] % 10);
    }

    if (Size > 6) {
        long_val = (long)(Data[6]);
    }
    if (Size > 7) {
        long_val = (long)(Data[6] * 256 + Data[7]);
    }

    if (Size > 8) {
        str_val = (const char *)(Data + 8);
    }

    /* Ensure string is null-terminated for safety */
    char safe_str[256];
    size_t str_len = 0;
    if (Size > 8) {
        str_len = Size - 8;
        if (str_len >= sizeof(safe_str)) str_len = sizeof(safe_str) - 1;
        memcpy(safe_str, Data + 8, str_len);
        safe_str[str_len] = '\0';
        str_val = safe_str;
    }

    switch (op) {
        case 0: {
            /* Fuzz curl_multi_wakeup */
            curl_multi_wakeup(multi);
            break;
        }
        case 1: {
            /* Fuzz curl_multi_assign */
            curl_multi_assign(multi, sockfd, sockp);
            break;
        }
        case 2: {
            /* Fuzz curl_multi_remove_handle */
            curl_multi_remove_handle(multi, easy);
            break;
        }
        case 3: {
            /* Fuzz curl_multi_setopt */
            if (option == CURLMOPT_MAXCONNECTS) {
                curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)long_val);
            } else if (option == CURLMOPT_PIPELINING) {
                curl_multi_setopt(multi, CURLMOPT_PIPELINING, (long)long_val);
            } else if (option == CURLMOPT_PUSHFUNCTION) {
                curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, NULL);
            } else {
                curl_multi_setopt(multi, option, (long)long_val);
            }
            break;
        }
        case 4: {
            /* Fuzz curl_multi_strerror */
            CURLMcode code = 0;
            if (Size > 1) {
                code = (CURLMcode)(Data[1]);
            }
            if (Size > 2) {
                code = (CURLMcode)(Data[1] * 256 + Data[2]);
            }
            const char *msg = curl_multi_strerror(code);
            (void)msg;
            break;
        }
        case 5: {
            /* Combined operations to explore state */
            curl_multi_wakeup(multi);
            curl_multi_assign(multi, sockfd, sockp);
            curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)long_val);
            curl_multi_remove_handle(multi, easy);
            break;
        }
    }

    /* Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}