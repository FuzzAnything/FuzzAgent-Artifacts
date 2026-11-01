// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <unistd.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets:
 * - curl_multi_socket_action
 * - curl_multi_socket_all
 * - curl_multi_socket
 * - curl_multi_perform
 */

/*
 * Helper to create a dummy file with fuzz data if needed.
 */
static void create_dummy_file(const uint8_t *data, size_t size) {
    if (size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

/*
 * Simple write callback for curl_easy_setopt to prevent crashes.
 */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/*
 * Simple read callback for curl_easy_setopt.
 */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Set up basic options to make the handle somewhat valid */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_cb);
    
    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi, easy) != CURLM_OK) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Prepare variables for running_handles */
    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_BAD;
    int ev_bitmask = 0;

    /* Use fuzz data to derive parameters safely */
    if (Size >= 5) {
        s = (curl_socket_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        ev_bitmask = Data[4] % 4; /* CURL_CSELECT_IN, OUT, ERR, TIMEOUT */
    } else if (Size >= 1) {
        /* Fallback for small inputs */
        s = (curl_socket_t)Data[0];
        ev_bitmask = 0;
    }

    /* Create a dummy file if data is large enough to simulate file operations */
    if (Size > 100) {
        create_dummy_file(Data, Size);
    }

    /*
     * Fuzz curl_multi_socket_action
     * This is the primary target.
     */
    (void)curl_multi_socket_action(multi, s, ev_bitmask, &running_handles);

    /*
     * Fuzz curl_multi_socket_all (deprecated)
     */
    (void)curl_multi_socket_all(multi, &running_handles);

    /*
     * Fuzz curl_multi_socket (deprecated)
     */
    (void)curl_multi_socket(multi, s, &running_handles);

    /*
     * Fuzz curl_multi_perform
     */
    (void)curl_multi_perform(multi, &running_handles);

    /*
     * Cleanup
     */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    /* Clean up dummy file if created */
    unlink("./dummy_file");

    return 0;
}