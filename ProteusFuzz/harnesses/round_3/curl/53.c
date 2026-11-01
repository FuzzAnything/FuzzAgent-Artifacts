// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
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

    // Create a dummy socket for testing curl_multi_assign
    int dummy_socket = -1;
    #ifdef _WIN32
    dummy_socket = -1; 
    #else
    dummy_socket = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    void *custom_data = (void *)(intptr_t)Data[0];

    // Fuzz curl_multi_assign
    if (dummy_socket >= 0) {
        curl_multi_assign(multi_handle, (curl_socket_t)dummy_socket, custom_data);
        close(dummy_socket);
    }

    // Fuzz curl_multi_add_handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_setopt with various options
    // Ensure we have enough data before accessing indices
    if (Size > 1) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (int)Data[1]);
    }
    if (Size > 2) {
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)Data[2]);
    }
    
    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_strerror with various error codes
    if (Size > 3) {
        CURLMcode error_code = (CURLMcode)Data[3];
        const char *error_str = curl_multi_strerror(error_code);
        if (error_str) {
            (void)strlen(error_str);
        }
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}