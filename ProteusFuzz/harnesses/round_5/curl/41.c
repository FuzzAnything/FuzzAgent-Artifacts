// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Fuzz driver for curl multi-handle socket functions.
 * Targets:
 * - curl_multi_socket_action
 * - curl_multi_socket (deprecated)
 * - curl_multi_socket_all (deprecated)
 * - curl_multi_assign
 * - curl_multi_strerror
 * - curl_multi_perform
 */

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    CURLM *multi = NULL;
    CURLMcode res;
    int running_handles = 0;
    curl_socket_t s = 0;
    int ev_bitmask = 0;
    const char *err_str;
    uint8_t val0 = 0;
    uint8_t val1 = 0;

    /* Basic sanity check */
    if (Size == 0) {
        return 0;
    }

    /* Write data to dummy file for potential file-based operations */
    write_dummy_file(Data, Size);

    /* Initialize multi handle */
    multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    /* Derive socket and bitmask from input data to explore states */
    if (Size >= 4) {
        s = (curl_socket_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
        ev_bitmask = (int)((Data[4 % Size] << 8) | (Data[(4 + 1) % Size]));
    }

    /* Test curl_multi_assign */
    void *sockp = (void *)(intptr_t)(Data[0] & 0xFF);
    res = curl_multi_assign(multi, s, sockp);
    (void)res; /* Ignore result for fuzzing, just trigger logic */

    /* Test curl_multi_perform */
    res = curl_multi_perform(multi, &running_handles);
    (void)res;

    /* Test curl_multi_socket_action */
    res = curl_multi_socket_action(multi, s, ev_bitmask, &running_handles);
    (void)res;

    /* Test deprecated curl_multi_socket */
    res = curl_multi_socket(multi, s, &running_handles);
    (void)res;

    /* Test deprecated curl_multi_socket_all */
    res = curl_multi_socket_all(multi, &running_handles);
    (void)res;

    /* Test curl_multi_strerror with various codes */
    err_str = curl_multi_strerror(CURLM_OK);
    (void)err_str;
    err_str = curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)err_str;
    err_str = curl_multi_strerror(CURLM_OUT_OF_MEMORY);
    (void)err_str;
    
    /* Safe access for fuzzing error codes */
    val0 = Data[0];
    if (Size > 1) {
        val1 = Data[1];
    }
    
    err_str = curl_multi_strerror((CURLMcode)val0);
    (void)err_str;
    err_str = curl_multi_strerror((CURLMcode)(val1 << 8));
    (void)err_str;

    /* Cleanup */
    curl_multi_cleanup(multi);

    return 0;
}