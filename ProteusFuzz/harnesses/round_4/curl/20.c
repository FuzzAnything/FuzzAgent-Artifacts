// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Parse input data to drive fuzzing logic
    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(uint32_t));
    uint32_t option_seed = (seed >> 8) & 0xFF;
    uint32_t action_seed = (seed >> 16) & 0xFF;

    // 1. curl_multi_setopt: Configure multi handle options
    // We try to set various options based on the seed.
    // Ensure we have enough data before accessing Data[4]
    if (Size > 4) {
        // Try setting a simple long option (e.g., PIPELINING) if seed suggests it
        if (option_seed % 3 == 0) {
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Data[4] & 0xFF));
        }
        // Try setting MAXCONNECTS
        if (option_seed % 3 == 1) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Data[4] & 0xFF));
        }
        // Try setting MAX_HOST_CONNECTIONS
        if (option_seed % 3 == 2) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Data[4] & 0xFF));
        }
    }

    // 2. curl_multi_wakeup: Attempt to wake up (safe to call even if not sleeping)
    if (action_seed % 2 == 0) {
        curl_multi_wakeup(multi_handle);
    }

    // 3. curl_multi_assign: Assign a socket to a pointer
    // We use a dummy socket value derived from input
    // Ensure we have enough data before accessing Data[4] and Data[5]
    if (Size > 5) {
        curl_socket_t dummy_socket = (curl_socket_t)(Data[4] & 0xFF);
        void *dummy_ptr = (void *)(uintptr_t)(Data[5] & 0xFF);
        curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);
    }

    // 4. curl_multi_remove_handle: Remove the easy handle
    // Ensure we add it first to make removal meaningful, though removal of non-added is safe
    curl_multi_add_handle(multi_handle, easy_handle);
    curl_multi_remove_handle(multi_handle, easy_handle);

    // 5. curl_multi_strerror: Convert error codes to strings
    // Iterate through a few potential error codes based on input
    for (size_t i = 0; i < Size && i < 10; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *msg = curl_multi_strerror(code);
        if (msg) {
            // Use the string to prevent compiler optimization of unused result
            volatile const char *v = msg;
            (void)v;
        }
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}