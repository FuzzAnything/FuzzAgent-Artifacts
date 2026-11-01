// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file();

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Set a dummy URL to ensure the handle is somewhat valid
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_PRIVATE, (void*)Data);
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, Size);

    CURLMcode rc;
    rc = curl_multi_add_handle(multi_handle, easy_handle);
    if (rc != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sock = -1;
    int ev_bitmask = 0;
    void *sockp = (void*)Data;

    // Fuzz curl_multi_perform
    rc = curl_multi_perform(multi_handle, &running_handles);
    (void)rc;

    // Fuzz curl_multi_socket_action with various socket and event combinations
    if (Size > 0) {
        sock = (curl_socket_t)(Data[0] & 0xFF);
        ev_bitmask = (Size > 1) ? (int)(Data[1] & 0xFF) : 0;
        rc = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
        (void)rc;
    }

    // Fuzz curl_multi_socket (deprecated)
    if (Size > 0) {
        sock = (curl_socket_t)(Data[0] & 0xFF);
        rc = curl_multi_socket(multi_handle, sock, &running_handles);
        (void)rc;
    }

    // Fuzz curl_multi_socket_all (deprecated)
    rc = curl_multi_socket_all(multi_handle, &running_handles);
    (void)rc;

    // Fuzz curl_multi_assign
    if (Size > 0) {
        sock = (curl_socket_t)(Data[0] & 0xFF);
        rc = curl_multi_assign(multi_handle, sock, sockp);
        (void)rc;
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}