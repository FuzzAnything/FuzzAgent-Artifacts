// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 7) {
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

    // Fuzz curl_multi_add_handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_setopt
    // Use long for integer options to satisfy type checking macros
    long max_connects = Data[0] % 10 + 1;
    long max_host_conn = Data[2] % 5 + 1;
    long max_total_conn = Data[3] % 5 + 1;
    
    // CURLMOPT_MAXCONNECTS
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
    
    // CURLMOPT_PIPELINING - use integer values directly as the enum values
    // CURLPIPE_MULTI is 1, CURLPIPE_SINGLE is 2
    int pipelining_mode = (Data[1] % 2) ? 1 : 2; 
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining_mode);

    // CURLMOPT_MAX_HOST_CONNECTIONS
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conn);
    
    // CURLMOPT_MAX_TOTAL_CONNECTIONS
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_total_conn);

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Re-add to test re-adding scenarios
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_assign with a dummy socket and pointer
    curl_socket_t sock = (curl_socket_t)(Data[4] % 100);
    void *sockp = (void *)(size_t)(Data[5] % 1000);
    curl_multi_assign(multi_handle, sock, sockp);

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_code = (CURLMcode)(Data[6] % 20);
    const char *str = curl_multi_strerror(error_code);
    if (str) {
        // Use the string to avoid unused variable warnings
        (void)strlen(str);
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}