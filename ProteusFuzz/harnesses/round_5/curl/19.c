// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Helper function to initialize a CURLM handle safely.
 * Returns NULL if initialization fails.
 */
static CURLM *init_multi_handle(void) {
    return curl_multi_init();
}

/*
 * Helper function to cleanup a CURLM handle.
 */
static void cleanup_multi_handle(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

/*
 * Helper function to create a dummy file if needed.
 */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for curl testing\n");
        fclose(f);
    }
}

/*
 * Fuzzing entry point.
 * Data: Raw fuzz input.
 * Size: Size of the input data.
 */
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create dummy file for potential file-based options */
    create_dummy_file();

    /* Initialize multi handle */
    CURLM *multi = init_multi_handle();
    if (!multi) {
        return 0;
    }

    /*
     * 1. Test curl_multi_strerror
     * We can fuzz the error code by interpreting the input data as an integer.
     * Use modulo to keep it within a reasonable range or just cast directly.
     */
    if (Size >= sizeof(CURLMcode)) {
        CURLMcode error_code = *(const CURLMcode *)Data;
        const char *str = curl_multi_strerror(error_code);
        (void)str; /* Suppress unused variable warning */
    } else {
        /* Fallback if data is too small */
        CURLMcode error_code = (CURLMcode)(Size * 100);
        const char *str = curl_multi_strerror(error_code);
        (void)str;
    }

    /*
     * 2. Test curl_multi_wakeup
     * This function is meant to wake up a sleeping poll.
     * We call it directly on the handle.
     */
    CURLMcode wakeup_res = curl_multi_wakeup(multi);
    (void)wakeup_res;

    /*
     * 3. Test curl_multi_waitfds
     * We need to allocate a buffer for the waitfds.
     * Use the input data to determine the size, but cap it to prevent OOM.
     */
    unsigned int fd_count = 0;
    unsigned int size = 0;
    if (Size >= 2) {
        size = Data[0] | (Data[1] << 8);
        /* Cap size to a reasonable number to avoid allocation issues */
        if (size > 1000) size = 1000;
    }

    struct curl_waitfd *ufds = NULL;
    if (size > 0) {
        ufds = (struct curl_waitfd *)malloc(size * sizeof(struct curl_waitfd));
        if (ufds) {
            /* Initialize the array with some dummy data derived from fuzz input */
            for (unsigned int i = 0; i < size; i++) {
                ufds[i].fd = (curl_socket_t)(i % 256);
                ufds[i].events = (short)(i % 10);
                ufds[i].revents = 0;
            }
            CURLMcode waitfds_res = curl_multi_waitfds(multi, ufds, size, &fd_count);
            (void)waitfds_res;
            (void)fd_count;
        }
    }
    free(ufds);

    /*
     * 4. Test curl_multi_notify_disable
     * Use fuzz data to determine the notification flags.
     */
    unsigned int notify_flags = 0;
    if (Size >= 4) {
        notify_flags = *(const unsigned int *)(Data + Size - 4);
    } else {
        notify_flags = Size;
    }
    CURLMcode notify_disable_res = curl_multi_notify_disable(multi, notify_flags);
    (void)notify_disable_res;

    /*
     * 5. Test curl_multi_notify_enable
     * Use a different subset of data or the same flags.
     */
    unsigned int notify_enable_flags = 0;
    if (Size >= 4) {
        notify_enable_flags = *(const unsigned int *)(Data + Size - 4) ^ 0xFF;
    } else {
        notify_enable_flags = Size ^ 0xFF;
    }
    CURLMcode notify_enable_res = curl_multi_notify_enable(multi, notify_enable_flags);
    (void)notify_enable_res;

    /*
     * 6. Attempt to use Wcurl_easy_setopt_err_curl macros indirectly.
     * Since these are compile-time checks, we cannot "fuzz" them at runtime directly.
     * However, we can ensure the environment is set up such that if we were to
     * write code using them, the types would be valid.
     * We create a CURL handle and set a valid option to ensure the library is initialized.
     */
    CURL *easy = curl_easy_init();
    if (easy) {
        /* Set a valid string option to exercise the library */
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        /* Set a valid long option */
        curl_easy_setopt(easy, CURLOPT_TIMEOUT, 10L);
        
        /* Add to multi handle to ensure multi handle is active */
        curl_multi_add_handle(multi, easy);
        
        /* Perform a dummy operation to exercise the multi handle state */
        int running = 0;
        curl_multi_perform(multi, &running);
        
        /* Remove the handle */
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }

    /* Cleanup */
    cleanup_multi_handle(multi);

    return 0;
}