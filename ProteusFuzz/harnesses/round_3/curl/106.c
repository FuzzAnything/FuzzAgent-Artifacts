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
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
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

static void setup_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    // Setup a dummy file if needed (though not strictly used by the target APIs, 
    // it satisfies the requirement to handle file scenarios if options were set)
    setup_dummy_file("./dummy_file", Data, Size);

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi, easy) != CURLM_OK) {
        // If add fails, we can still test remove with invalid state
    }

    // Fuzz curl_multi_wakeup
    // This function expects a valid multi handle. We pass the valid one.
    // It is designed to be called from another thread, but for fuzzing we call it directly.
    curl_multi_wakeup(multi);

    // Fuzz curl_multi_remove_handle
    // Try removing the easy handle
    curl_multi_remove_handle(multi, easy);

    // Try removing again (should return CURLM_OK or handle already removed state)
    curl_multi_remove_handle(multi, easy);

    // Try removing with an invalid easy handle (NULL)
    curl_multi_remove_handle(multi, NULL);

    // Try removing with an invalid multi handle (NULL)
    curl_multi_remove_handle(NULL, easy);

    // Fuzz curl_multi_setopt
    // Set various options to explore state space
    // CURLMOPT_MAXCONNECTS
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)Size);
    // CURLMOPT_MAX_HOST_CONNECTIONS
    curl_multi_setopt(multi, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Size % 100));
    // CURLMOPT_PIPELINING
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, (long)(Size % 3));
    // CURLMOPT_PUSHFUNCTION (setting to NULL disables push)
    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, NULL);
    // Invalid option to test error handling
    curl_multi_setopt(multi, (CURLMoption)0x999999, 0);
    // Invalid multi handle
    curl_multi_setopt(NULL, CURLMOPT_MAXCONNECTS, 10);

    // Fuzz curl_multi_strerror
    // Test with known error codes
    const char *str1 = curl_multi_strerror(CURLM_OK);
    const char *str2 = curl_multi_strerror(CURLM_BAD_HANDLE);
    const char *str3 = curl_multi_strerror(CURLM_WAKEUP_FAILURE);
    const char *str4 = curl_multi_strerror(CURLM_BAD_EASY_HANDLE);
    const char *str5 = curl_multi_strerror(CURLM_INTERNAL_ERROR);
    
    // Test with fuzzed value cast to CURLMcode (might be invalid enum)
    CURLMcode fuzzed_code = (CURLMcode)(Size % 1000);
    const char *str6 = curl_multi_strerror(fuzzed_code);

    // Ensure we don't crash on NULL pointer dereference in error strings if any
    (void)str1;
    (void)str2;
    (void)str3;
    (void)str4;
    (void)str5;
    (void)str6;

    // Cleanup
    // Remove handle if not already removed (idempotent check)
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    return 0;
}