// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
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
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Target APIs:
 * 1. curl_multi_strerror
 * 2. curl_multi_setopt
 * 3. curl_multi_wakeup
 * 4. curl_multi_remove_handle
 */

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    CURLM *multi = NULL;
    CURL *easy = NULL;
    CURLMcode rc;
    const char *err_str;
    int option_idx;
    long val_long;
    void *ptr_val;

    if (Size < 2) {
        return 0;
    }

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a multi handle */
    multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to associate with multi */
    easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    option_idx = Data[0] % 10;
    val_long = (long)((Data[0] << 8) | Data[1]);
    ptr_val = (void *)(size_t)val_long;

    /*
     * 1. Fuzz curl_multi_strerror with various error codes.
     */
    for (int i = 0; i < 10 && i < Size; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        err_str = curl_multi_strerror(code);
        if (err_str) {
            (void)strlen(err_str);
        }
    }

    /*
     * 2. Fuzz curl_multi_setopt with various options.
     */
    switch (option_idx) {
        case 0:
            rc = curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, val_long);
            break;
        case 1:
            rc = curl_multi_setopt(multi, CURLMOPT_PIPELINING, val_long);
            break;
        case 2:
            rc = curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, NULL);
            break;
        case 3:
            rc = curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, NULL);
            break;
        case 4:
            rc = curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, NULL);
            break;
        case 5:
            rc = curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, ptr_val);
            break;
        case 6:
            rc = curl_multi_setopt(multi, CURLMOPT_TIMERDATA, ptr_val);
            break;
        case 7:
            rc = curl_multi_setopt(multi, CURLMOPT_PUSHDATA, ptr_val);
            break;
        case 8:
            rc = curl_multi_setopt(multi, (CURLMoption)0xFFFF, val_long);
            break;
        default:
            rc = curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, val_long);
            break;
    }

    /*
     * 3. Fuzz curl_multi_wakeup.
     */
    rc = curl_multi_wakeup(multi);

    /*
     * 4. Fuzz curl_multi_remove_handle.
     */
    rc = curl_multi_add_handle(multi, easy);
    if (rc == CURLM_OK) {
        rc = curl_multi_remove_handle(multi, easy);
    }

    /* Additional state exploration */
    rc = curl_multi_remove_handle(multi, easy);
    rc = curl_multi_remove_handle(multi, NULL);
    rc = curl_multi_remove_handle(NULL, easy);

    /* Cleanup */
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    curl_global_cleanup();

    return 0;
}