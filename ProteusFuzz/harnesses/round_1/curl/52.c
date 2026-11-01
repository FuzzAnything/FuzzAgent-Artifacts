// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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

/* Static helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *dummy_data = "dummy data for curl testing";
        fwrite(dummy_data, 1, strlen(dummy_data), f);
        fclose(f);
    }
}

/* Static callback for easy handle write function */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

/* Static callback for easy handle read function */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare environment */
    create_dummy_file();

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Set some basic options to make the handle usable */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, Size);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Extract fuzzing data for parameters */
    int running_handles = 0;
    curl_socket_t s = -1;
    int ev_bitmask = 0;

    if (Size > 0) {
        s = (curl_socket_t)Data[0];
    }
    if (Size > 1) {
        ev_bitmask = Data[1];
    }
    if (Size > 2) {
        running_handles = Data[2];
    }

    /* Fuzz curl_multi_socket_action */
    curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);

    /* Fuzz deprecated curl_multi_socket */
    curl_multi_socket(multi_handle, s, &running_handles);

    /* Fuzz deprecated curl_multi_socket_all */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* Fuzz curl_multi_perform */
    curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_assign with various socket values */
    curl_socket_t assign_sock = -1;
    if (Size > 3) {
        assign_sock = (curl_socket_t)Data[3];
    }
    void *sockp = (void *)(uintptr_t)Data[0];
    curl_multi_assign(multi_handle, assign_sock, sockp);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}