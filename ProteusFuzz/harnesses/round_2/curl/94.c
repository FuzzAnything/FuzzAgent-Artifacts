// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void fuzz_multi_strerror(const uint8_t *data, size_t size) {
    if (size < sizeof(CURLMcode)) {
        return;
    }
    CURLMcode code;
    memcpy(&code, data, sizeof(CURLMcode));
    const char *msg = curl_multi_strerror(code);
    (void)msg;
}

static void fuzz_multi_wakeup(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return;
    }
    curl_multi_wakeup(multi);
    curl_multi_cleanup(multi);
}

static void fuzz_multi_assign(const uint8_t *data, size_t size) {
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return;
    }
    if (size < sizeof(curl_socket_t)) {
        curl_multi_cleanup(multi);
        return;
    }
    curl_socket_t sockfd;
    memcpy(&sockfd, data, sizeof(curl_socket_t));
    void *sockp = (void *)(size > sizeof(curl_socket_t) ? (uintptr_t)data[sizeof(curl_socket_t)] : 0);
    curl_multi_assign(multi, sockfd, sockp);
    curl_multi_cleanup(multi);
}

static void fuzz_multi_remove_handle(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return;
    }
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return;
    }
    curl_multi_add_handle(multi, easy);
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
}

static void fuzz_multi_setopt(const uint8_t *data, size_t size) {
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return;
    }
    
    /* 
     * curl_multi_setopt is a macro that performs type checking.
     * To avoid compilation errors, we must pass arguments that match
     * the expected type for the specific option enum value.
     * We will only test with options we can safely construct arguments for.
     */
    
    if (size >= sizeof(CURLMoption)) {
        CURLMoption option;
        memcpy(&option, data, sizeof(CURLMoption));
        
        switch (option) {
            case CURLMOPT_MAXCONNECTS: {
                int val = 10;
                curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, val);
                break;
            }
            case CURLMOPT_PIPELINING: {
                long val = 1;
                curl_multi_setopt(multi, CURLMOPT_PIPELINING, val);
                break;
            }
            case CURLMOPT_PUSHFUNCTION: {
                /* Pass NULL for the function pointer */
                curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, NULL);
                break;
            }
            case CURLMOPT_PUSHDATA: {
                /* Pass a dummy pointer */
                curl_multi_setopt(multi, CURLMOPT_PUSHDATA, (void*)0x12345678);
                break;
            }
            default:
                /* For unknown or complex options, skip to avoid macro expansion errors */
                break;
        }
    }
    curl_multi_cleanup(multi);
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    fuzz_multi_strerror(Data, Size);
    fuzz_multi_wakeup(Data, Size);
    fuzz_multi_assign(Data, Size);
    fuzz_multi_remove_handle(Data, Size);
    fuzz_multi_setopt(Data, Size);
    return 0;
}