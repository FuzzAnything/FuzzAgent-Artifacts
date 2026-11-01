// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
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

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize libcurl global environment
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle to associate with the multi handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to ensure the handle is valid for operations
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);

    // Add the easy handle to the multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Extract data from fuzz input for testing
    size_t idx = 0;
    int error_code_int = 0;
    int option_int = 0;
    int long_val = 0;

    if (idx < Size) {
        error_code_int = Data[idx++];
    }
    if (idx < Size) {
        option_int = Data[idx++];
    }
    if (idx < Size) {
        long_val = Data[idx++];
    }

    // 1. Test curl_multi_strerror
    // Convert the fuzzed byte to a CURLMcode. 
    CURLMcode error_code = (CURLMcode)error_code_int;
    const char *str = curl_multi_strerror(error_code);
    if (str) {
        (void)str;
    }

    // 2. Test curl_multi_wakeup
    CURLM *wakeup_handle = multi_handle;
    if (Size > 10 && Data[10] == 0) {
        wakeup_handle = NULL;
    }
    CURLMcode wakeup_res = curl_multi_wakeup(wakeup_handle);
    (void)wakeup_res;

    // 3. Test curl_multi_remove_handle
    CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_res;

    // 4. Test curl_multi_setopt
    // We use only valid CURLMoption constants to avoid compilation errors.
    // CURLMOPT_MAXCONNECTS (takes long)
    if (option_int == 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)long_val);
    }
    // CURLMOPT_PIPELINING (takes long, 0 or 1)
    else if (option_int == 1) {
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(long_val & 1));
    }
    // CURLMOPT_MAX_HOST_CONNECTIONS (takes long)
    else if (option_int == 2) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)long_val);
    }
    // CURLMOPT_MAX_PIPELINE_LENGTH (takes long)
    else if (option_int == 3) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, (long)long_val);
    }
    // CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE (takes long)
    else if (option_int == 4) {
        curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, (long)long_val);
    }
    // CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE (takes long)
    else if (option_int == 5) {
        curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, (long)long_val);
    }
    // Try an invalid option to test error handling (cast to avoid compile error if undefined)
    else {
        curl_multi_setopt(multi_handle, (CURLMoption)option_int, 0L);
    }

    // Cleanup easy handle
    curl_easy_cleanup(easy_handle);

    // Cleanup multi handle
    curl_multi_cleanup(multi_handle);

    // Cleanup global
    curl_global_cleanup();

    return 0;
}