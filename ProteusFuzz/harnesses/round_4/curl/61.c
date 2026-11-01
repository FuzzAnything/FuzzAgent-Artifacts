// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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

static void cleanup_handles(CURLM *multi_handle) {
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURLMcode res;
    const char *str_res;

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Use input data to drive fuzzing logic
    uint8_t mode = Data[0] % 10;

    switch (mode) {
        case 0:
        case 1:
        case 2:
            // Fuzz curl_multi_wakeup with valid handle
            res = curl_multi_wakeup(multi_handle);
            (void)res; // Suppress unused warning
            break;

        case 3:
        case 4:
            // Fuzz curl_multi_wakeup with NULL handle (invalid)
            res = curl_multi_wakeup(NULL);
            (void)res;
            break;

        case 5:
        case 6:
            // Fuzz curl_multi_setopt with various options
            if (Size > 1) {
                int option_val = Data[1] % 10; // Simplified option selection
                // Example: Setting a dummy option (CURLMOPT_MAXCONNECTS is a valid long option)
                // We use a safe default option for fuzzing to avoid crashes from invalid options
                res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Data[1] & 0xFF));
                (void)res;
            }
            break;

        case 7:
        case 8:
            // Fuzz curl_multi_strerror with various error codes
            if (Size > 2) {
                CURLMcode error_code = (CURLMcode)(Data[2] & 0xFF);
                str_res = curl_multi_strerror(error_code);
                (void)str_res;
            }
            break;

        case 9:
            // Fuzz curl_multi_setopt with NULL handle
            if (Size > 1) {
                res = curl_multi_setopt(NULL, CURLMOPT_MAXCONNECTS, (long)(Data[1] & 0xFF));
                (void)res;
            }
            break;

        default:
            // Default: just call wakeup
            res = curl_multi_wakeup(multi_handle);
            (void)res;
            break;
    }

    // Cleanup
    cleanup_handles(multi_handle);
    curl_global_cleanup();

    return 0;
}