// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
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

    // Add easy handle to multi handle to ensure valid state
    curl_multi_add_handle(multi_handle, easy_handle);

    // Create a socket for testing curl_multi_assign
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        sockfd = -1;
    }

    // Use input data to drive fuzzing logic safely
    uint8_t op = Data[0] % 6;
    uint8_t socket_val = Data[1];
    uint8_t error_code = Data[2];
    uint8_t val4 = Data[3];
    uint8_t val5 = Data[4];
    uint8_t val6 = Data[5];

    void *sockp = (void *)(uintptr_t)socket_val;

    switch (op) {
        case 0:
            // Fuzz curl_multi_assign
            if (sockfd >= 0) {
                curl_multi_assign(multi_handle, sockfd, sockp);
            } else {
                curl_multi_assign(multi_handle, -1, sockp);
            }
            break;

        case 1:
            // Fuzz curl_multi_strerror
            (void)curl_multi_strerror((CURLMcode)error_code);
            (void)curl_multi_strerror(CURLM_OK);
            (void)curl_multi_strerror(CURLM_BAD_HANDLE);
            (void)curl_multi_strerror(CURLM_BAD_EASY_HANDLE);
            break;

        case 2:
            // Fuzz curl_multi_remove_handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_remove_handle(multi_handle, easy_handle);
            {
                CURLM *bad_multi = curl_multi_init();
                if (bad_multi) {
                    curl_multi_remove_handle(bad_multi, easy_handle);
                    curl_multi_cleanup(bad_multi);
                }
            }
            break;

        case 3:
            // Fuzz curl_multi_wakeup
            curl_multi_wakeup(multi_handle);
            break;

        case 4:
            // Fuzz curl_multi_setopt
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(val4 % 100));
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(val5 % 10));
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(val6 % 2));
            curl_multi_setopt(multi_handle, (CURLMoption)(val4 % 1000), 0);
            break;

        case 5:
            // Combined operations
            if (sockfd >= 0) {
                curl_multi_assign(multi_handle, sockfd, sockp);
            }
            curl_multi_wakeup(multi_handle);
            curl_multi_remove_handle(multi_handle, easy_handle);
            break;
    }

    // Cleanup
    if (sockfd >= 0) {
        close(sockfd);
    }
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}