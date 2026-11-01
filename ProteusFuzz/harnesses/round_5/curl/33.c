// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Static helper to safely initialize curl_global_init if needed.
 */
static void ensure_global_init(void) {
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    (void)res;
}

/*
 * Static helper to create a dummy easy handle for testing remove_handle.
 */
static CURL *create_dummy_easy_handle(void) {
    return curl_easy_init();
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    ensure_global_init();

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Create an easy handle to potentially add/remove */
    CURL *easy_handle = create_dummy_easy_handle();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /*
     * Fuzzing logic:
     * We interpret the input data to drive various API calls and options.
     * We must strictly check Size before accessing Data[i].
     */

    /* 1. Test curl_multi_setopt */
    /* We will try to set CURLMOPT_PUSHFUNCTION and other safe options */
    if (Size >= 1) {
        int option_choice = Data[0] % 3;
        CURLMcode res;

        if (option_choice == 0) {
            /* Try setting CURLMOPT_PUSHFUNCTION */
            /* Passing NULL disables the callback. */
            res = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
            (void)res;
        } else if (option_choice == 1) {
            /* Try setting CURLMOPT_MAXCONNECTS */
            if (Size >= 3) {
                long max_conn = (long)(Data[1] | (Data[2] << 8));
                res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conn);
                (void)res;
            }
        } else {
            /* Try setting CURLMOPT_PIPELINING */
            if (Size >= 2) {
                long pipelining = (long)(Data[1] % 3);
                res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
                (void)res;
            }
        }
    }

    /* 2. Test curl_multi_strerror */
    /* We can generate various error codes from the input */
    if (Size >= 3) {
        CURLMcode error_code = (CURLMcode)(Data[1] | (Data[2] << 8));
        const char *str = curl_multi_strerror(error_code);
        if (str) {
            (void)strlen(str);
        }
    }

    /* 3. Test curl_multi_assign */
    /* Requires a socket and a pointer. We'll use a dummy socket and pointer. */
    if (Size >= 6) {
        curl_socket_t sock = (curl_socket_t)(Data[2] | (Data[3] << 8));
        void *sockp = (void *)(uintptr_t)(Data[4] | (Data[5] << 8));
        CURLMcode res_assign = curl_multi_assign(multi_handle, sock, sockp);
        (void)res_assign;
    }

    /* 4. Test curl_multi_remove_handle */
    /* Add the easy handle first, then remove it */
    if (Size >= 4) {
        /* Add handle */
        CURLMcode res_add = curl_multi_add_handle(multi_handle, easy_handle);
        (void)res_add;

        /* Remove handle */
        CURLMcode res_remove = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)res_remove;
    }

    /* 5. Test curl_multi_wakeup */
    CURLMcode res_wakeup = curl_multi_wakeup(multi_handle);
    (void)res_wakeup;

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}