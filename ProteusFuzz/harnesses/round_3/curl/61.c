// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
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
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
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
#include <fcntl.h>
#include <unistd.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode res;
    curl_socket_t dummy_socket = -1;
    void *dummy_ptr = NULL;
    const char *dummy_file = "./dummy_file";

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to configure easy handle if possible */
    if (Size > 0) {
        /* Set URL to a dummy value derived from input or fixed */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        /* Use input as POST data if it looks safe (non-null terminated might be okay for curl) */
        if (Size > 0 && Size < 1024) {
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, Data);
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, Size);
        }
        /* Add to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* Fuzz curl_multi_wakeup */
    /* This function is meant to be called from another thread, but for fuzzing we call it directly */
    res = curl_multi_wakeup(multi_handle);
    (void)res; /* Ignore result for fuzzing, just exercise code path */

    /* Fuzz curl_multi_setopt */
    /* Try setting various options using input data if applicable */
    if (Size > 0) {
        /* Example: Set a long option if input is small enough to cast safely */
        long val_long = (long)(Data[0] & 0xFF);
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, val_long);
        (void)res;

        /* Example: Set a callback option (using a dummy callback or NULL) */
        res = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
        (void)res;
    }

    /* Fuzz curl_multi_assign */
    /* Create a dummy socket for testing */
    int pipefd[2];
    if (pipe(pipefd) == 0) {
        dummy_socket = pipefd[0];
        dummy_ptr = (void *)(uintptr_t)(Data[0] & 0xFF); /* Use input to generate pointer */
        res = curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);
        (void)res;
        close(pipefd[0]);
        close(pipefd[1]);
    }

    /* Fuzz curl_multi_remove_handle */
    res = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)res;

    /* Fuzz curl_multi_strerror */
    /* Test with various error codes derived from input */
    if (Size > 0) {
        CURLMcode error_code = (CURLMcode)(Data[0] & 0xFF);
        const char *err_str = curl_multi_strerror(error_code);
        (void)err_str; /* Just exercise the function */
    }

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    curl_global_cleanup();

    /* Create dummy file if needed for other tests (not used here but good practice) */
    if (Size > 0) {
        create_dummy_file(dummy_file, Data, Size);
    }

    return 0;
}