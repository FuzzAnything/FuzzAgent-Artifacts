// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy callback to satisfy curl_multi_setopt requirements if needed */
static int dummy_push_callback(CURL *curl, struct curl_pushheaders *headers, void *userp) {
    (void)curl;
    (void)headers;
    (void)userp;
    return CURL_PUSH_OK;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* 1. Fuzz curl_multi_strerror with various error codes derived from input */
    CURLMcode error_code = (CURLMcode)(Data[0]);
    const char *str = curl_multi_strerror(error_code);
    if (str) {
        /* Consume the string to avoid unused variable warnings and ensure path execution */
        size_t len = strlen(str);
        if (len > 0) {
            /* Just a dummy operation to use the result */
            volatile char c = str[0];
            (void)c;
        }
    }

    /* 2. Fuzz curl_multi_wakeup */
    /* According to instructions: ensure argument is valid handle or NULL */
    /* We test with the valid handle first */
    (void)curl_multi_wakeup(multi_handle);
    
    /* Test with NULL (if the API allows it, though docs imply valid handle) */
    /* Some versions might crash on NULL, but fuzzing should explore it safely if guarded.
       The instructions say "Ensure the argument is either a valid CURLM handle ... or explicitly NULL".
       We will try NULL only if Size > 1 and Data[1] indicates it, to diversify states. */
    if (Size > 1 && Data[1] == 0xFF) {
        (void)curl_multi_wakeup(NULL);
    }

    /* 3. Fuzz curl_multi_add_handle */
    /* Add the easy handle to the multi stack */
    (void)curl_multi_add_handle(multi_handle, easy_handle);

    /* Try adding again to trigger CURLM_ADDED_ALREADY */
    (void)curl_multi_add_handle(multi_handle, easy_handle);

    /* Try with NULL easy handle to trigger CURLM_BAD_EASY_HANDLE */
    (void)curl_multi_add_handle(multi_handle, NULL);

    /* Try with NULL multi handle to trigger CURLM_BAD_HANDLE */
    (void)curl_multi_add_handle(NULL, easy_handle);

    /* 4. Fuzz curl_multi_setopt */
    /* Set a valid option */
    (void)curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
    (void)curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, NULL);

    /* Fuzz with invalid option codes derived from input */
    if (Size > 2) {
        CURLMoption opt = (CURLMoption)(Data[2]);
        /* We pass NULL as the third argument for simplicity, 
           though some options require specific types. 
           This explores the error handling path for bad options. */
        (void)curl_multi_setopt(multi_handle, opt, NULL);
    }

    /* 5. Cleanup */
    /* Remove the handle before cleanup to avoid errors */
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}