// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Fuzz driver for curl_share_init and related type-checking macros.
 * The Wcurl_* macros are compile-time only and do not require runtime invocation.
 * We focus on fuzzing curl_share_init and ensuring proper cleanup.
 */

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Initialize curl global environment once per process.
     * In a real fuzzing harness, this might be done in a setup function,
     * but for simplicity, we do it here with a guard.
     */
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    /* 
     * Create a shared handle using curl_share_init.
     * This is the primary target API function.
     */
    CURLSH *share = curl_share_init();
    if (share == NULL) {
        /* Allocation failed, nothing to fuzz further in this iteration */
        return 0;
    }

    /* 
     * Optionally, we can try to set some options on the share handle
     * to explore more states, although curl_share_setopt is not listed
     * in the target APIs, it's a natural extension.
     * However, per instructions, we focus on the provided APIs.
     * Since curl_share_init is the only runtime API provided, we just
     * exercise it and clean up.
     */

    /* 
     * Cleanup: destroy the shared handle to prevent memory leaks.
     */
    curl_share_cleanup(share);

    return 0;
}