// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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

/*
 * Fuzz driver for curl multi-handle functions.
 * Target APIs:
 * - curl_multi_perform
 * - curl_multi_socket_all
 * - curl_multi_socket
 * - curl_multi_assign
 * - curl_multi_strerror
 *
 * Note: Wcurl_easy_setopt_err_opensocket_cb is a compile-time macro guard,
 * not a runtime function, so it is not invoked directly here.
 */

static void write_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *dummy = "dummy data for fuzzing";
        fwrite(dummy, 1, strlen(dummy), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure dummy file exists if needed */
    write_dummy_file();

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    int running_handles = 0;
    CURLMcode res;

    /*
     * Use input data to diversify calls:
     * - Interpret parts as socket descriptors (cast to curl_socket_t)
     * - Interpret parts as error codes (cast to CURLMcode)
     * - Interpret parts as private pointers for curl_multi_assign
     */

    /* 1. Test curl_multi_perform */
    res = curl_multi_perform(multi, &running_handles);
    (void)res; /* Ignore result, just exercise the call */

    /* 2. Test curl_multi_socket_all (deprecated) */
    res = curl_multi_socket_all(multi, &running_handles);
    (void)res;

    /* 3. Test curl_multi_socket (deprecated) with various socket values */
    if (Size >= 4) {
        curl_socket_t sock = (curl_socket_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        res = curl_multi_socket(multi, sock, &running_handles);
        (void)res;
    }

    /* 4. Test curl_multi_assign with various socket and private pointer values */
    if (Size >= 8) {
        curl_socket_t sockfd = (curl_socket_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
        void *sockp = (void *)(uintptr_t)(Data[0] | (Data[1] << 8)); /* Simple pointer derivation */
        res = curl_multi_assign(multi, sockfd, sockp);
        (void)res;
    }

    /* 5. Test curl_multi_strerror with various error codes */
    if (Size >= 2) {
        CURLMcode code = (CURLMcode)(Data[0] | (Data[1] << 8));
        const char *msg = curl_multi_strerror(code);
        if (msg) {
            /* Use the message to avoid unused variable warning */
            (void)msg;
        }
    }

    /* Cleanup */
    curl_multi_cleanup(multi);

    return 0;
}