// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
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

static void cleanup_handles(CURLM *multi, CURL *easy) {
    if (multi) {
        if (easy) {
            curl_multi_remove_handle(multi, easy);
        }
        curl_multi_cleanup(multi);
    }
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Fuzz curl_multi_strerror with various error codes
    if (Size >= 1) {
        CURLMcode error_code = (CURLMcode)Data[0];
        const char *msg = curl_multi_strerror(error_code);
        if (msg) {
            volatile const char *v = msg;
            (void)v;
        }
    }

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi);

    // Fuzz curl_multi_assign
    if (Size >= 2) {
        curl_socket_t sockfd = (curl_socket_t)(Data[1] << 8 | Data[0]);
        void *sockp = (void *)(uintptr_t)(Data[1] << 8 | Data[0]);
        curl_multi_assign(multi, sockfd, sockp);
    }

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi, easy);

    // Fuzz curl_multi_setopt with various options
    // We use curl_multi_setopt directly, avoiding the type-check macro issues
    // by casting the function pointer or using a wrapper if necessary.
    // However, since the macro is complex, we will just call it with valid types.
    if (Size >= 3) {
        CURLMoption option = (CURLMoption)(Data[2]);
        void *ptr = (void *)(uintptr_t)(Data[1] << 8 | Data[0]);
        long val = (long)(Data[1] << 8 | Data[0]);
        
        // We need to be careful with the macro. Let's just call it directly.
        // The macro expects specific types for specific options.
        // We'll try a few safe options.
        
        // Try setting a pointer option
        if (option == CURLMOPT_SOCKETDATA) {
            curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, ptr);
        } else if (option == CURLMOPT_TIMERDATA) {
            curl_multi_setopt(multi, CURLMOPT_TIMERDATA, ptr);
        } else if (option == CURLMOPT_MAXCONNECTS) {
            curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, val);
        } else if (option == CURLMOPT_PIPELINING) {
            curl_multi_setopt(multi, CURLMOPT_PIPELINING, val);
        } else {
            // For other options, we might need to handle them differently
            // or skip them to avoid type-check macro errors.
            // Let's try to cast the function to avoid the macro.
            // But since we can't easily do that, we'll just skip unknown options.
        }
    }

    // Cleanup
    cleanup_handles(multi, easy);

    return 0;
}