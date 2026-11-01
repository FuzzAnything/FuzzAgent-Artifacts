// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
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
 * Target functions:
 * - curl_multi_timeout
 * - curl_multi_get_offt
 */

static CURLM *create_multi_handle(void) {
    return curl_multi_init();
}

static void cleanup_multi_handle(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

/* Helper to add a dummy easy handle to the multi handle to make it valid for operations */
static CURL *create_dummy_easy_handle(CURLM *multi) {
    if (!multi) return NULL;
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy, CURLOPT_TIMEOUT, 1L);
        if (curl_multi_add_handle(multi, easy) != CURLM_OK) {
            curl_easy_cleanup(easy);
            return NULL;
        }
    }
    return easy;
}

static void cleanup_easy_handle(CURL *easy) {
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl if necessary */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURLM *multi = create_multi_handle();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Add a dummy easy handle to ensure the multi handle is in a valid state */
    CURL *easy = create_dummy_easy_handle(multi);
    if (!easy) {
        cleanup_multi_handle(multi);
        curl_global_cleanup();
        return 0;
    }

    /* 
     * Fuzzing curl_multi_timeout
     * Signature: CURLMcode curl_multi_timeout(CURLM *multi_handle, long *milliseconds);
     * Note: curl_multi_timeout does not accept NULL for the second argument in some versions
     * or may dereference it immediately. We must always pass a valid pointer.
     */
    long timeout_val = 0;
    long *timeout_ptr = &timeout_val;
    
    /* Case 1: Valid pointer */
    curl_multi_timeout(multi, timeout_ptr);

    /* Case 2: Fuzzed value in the variable, but valid pointer */
    if (Size >= sizeof(long)) {
        long fuzz_timeout;
        memcpy(&fuzz_timeout, Data, sizeof(long));
        curl_multi_timeout(multi, &fuzz_timeout);
    }

    /* 
     * Fuzzing curl_multi_get_offt
     * Signature: CURLMcode curl_multi_get_offt(CURLM *multi_handle, CURLMinfo_offt info, curl_off_t *pvalue);
     */
    curl_off_t offt_val = 0;
    curl_off_t *offt_ptr = &offt_val;

    /* Try valid info enums if known, or fuzz the enum value */
    CURLMinfo_offt info_enum = 0;
    
    if (Size >= sizeof(CURLMinfo_offt)) {
        memcpy(&info_enum, Data, sizeof(CURLMinfo_offt));
    }

    /* Case 1: Valid pointer */
    curl_multi_get_offt(multi, info_enum, offt_ptr);

    /* Case 2: Fuzzed pointer (using stack variable) */
    curl_off_t fuzz_offt = 0;
    if (Size >= sizeof(curl_off_t)) {
        memcpy(&fuzz_offt, Data, sizeof(curl_off_t));
    }
    curl_multi_get_offt(multi, info_enum, &fuzz_offt);

    /* 
     * Additional exploration: Try different enum values derived from input
     */
    for (size_t i = 0; i < Size && i < 10; i++) {
        CURLMinfo_offt fuzz_info = (CURLMinfo_offt)Data[i];
        curl_multi_get_offt(multi, fuzz_info, offt_ptr);
    }

    /* Cleanup */
    if (easy) {
        curl_multi_remove_handle(multi, easy);
        cleanup_easy_handle(easy);
    }
    cleanup_multi_handle(multi);
    curl_global_cleanup();

    return 0;
}