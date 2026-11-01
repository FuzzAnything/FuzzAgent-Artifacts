// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize libcurl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle to test remove_handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        // It's okay if this fails in fuzzing, we just proceed
    }

    // 1. Fuzz curl_multi_strerror
    // Extract a value from input to use as error code, ensuring bounds check
    CURLMcode error_code = (CURLMcode)0;
    if (Size >= 1) {
        error_code = (CURLMcode)Data[0];
    }
    if (Size >= 2) {
        error_code |= (CURLMcode)(Data[1] << 8);
    }
    const char *str = curl_multi_strerror(error_code);
    (void)str; // Use the result to avoid unused variable warnings

    // 2. Fuzz curl_multi_wakeup
    // Try with valid handle
    curl_multi_wakeup(multi_handle);
    // Try with NULL handle (should return CURLM_BAD_HANDLE)
    curl_multi_wakeup(NULL);

    // 3. Fuzz curl_multi_remove_handle
    // Remove the easy handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    // Try removing again (should be safe)
    curl_multi_remove_handle(multi_handle, easy_handle);
    // Try removing with NULL multi handle
    curl_multi_remove_handle(NULL, easy_handle);
    // Try removing with NULL easy handle
    curl_multi_remove_handle(multi_handle, NULL);

    // 4. Fuzz curl_multi_setopt
    // We can only safely set options that don't require complex callbacks for fuzzing
    // Try setting CURLMOPT_MAXCONNECTS
    long max_conns = 0;
    if (Size >= 4) {
        max_conns = (long)(Data[2] | (Data[3] << 8));
    }
    if (max_conns > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conns);
    }
    
    // Try setting CURLMOPT_PIPELINING
    long pipelining = 0;
    if (Size >= 6) {
        pipelining = (long)(Data[4] | (Data[5] << 8));
    }
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);

    // Try with NULL multi handle
    curl_multi_setopt(NULL, CURLMOPT_MAXCONNECTS, 10);

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}