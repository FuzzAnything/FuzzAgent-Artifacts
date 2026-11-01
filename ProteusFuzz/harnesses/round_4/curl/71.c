// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy file path as per instructions */
#define DUMMY_FILE "./dummy_file"

/* Static helper to write dummy data to file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen(DUMMY_FILE, "w");
    if (f) {
        fprintf(f, "dummy data for fuzzing\n");
        fclose(f);
    }
}

/* Static helper to clean up dummy file */
static void remove_dummy_file(void) {
    remove(DUMMY_FILE);
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data to work with */
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create dummy file if needed */
    create_dummy_file();

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        remove_dummy_file();
        return 0;
    }

    /* Initialize easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        remove_dummy_file();
        return 0;
    }

    /* Set a basic URL for the easy handle to avoid immediate errors */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        remove_dummy_file();
        return 0;
    }

    /* Fuzz curl_multi_wakeup */
    /* This function expects a valid multi handle; we pass it directly */
    (void)curl_multi_wakeup(multi_handle);

    /* Fuzz curl_multi_setopt with various options */
    /* Use data from input to select option type if possible */
    if (Size > 0) {
        int option = Data[0] % 10; /* Arbitrary selection */
        /* Example: Set CURLMOPT_MAXCONNECTS */
        if (option == 0) {
            long val = 1;
            if (Size > 1) {
                val = (long)(Data[1] % 100);
            }
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, val);
        }
        /* Example: Set CURLMOPT_PIPELINING */
        else if (option == 1) {
            long val = 0;
            if (Size > 1) {
                val = (long)(Data[1] % 2);
            }
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, val);
        }
        /* Add more options as needed */
    }

    /* Fuzz curl_multi_remove_handle */
    (void)curl_multi_remove_handle(multi_handle, easy_handle);

    /* Re-add easy handle for further testing */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        /* Handle error silently */
    }

    /* Fuzz curl_multi_assign */
    /* Assign a dummy socket and pointer */
    curl_socket_t dummy_socket = -1;
    void *dummy_ptr = (void *)(intptr_t)(Data[0] % 1000);
    (void)curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);

    /* Fuzz curl_multi_strerror with various error codes */
    if (Size > 1) {
        CURLMcode error_code = (CURLMcode)(Data[1] % 100);
        const char *error_str = curl_multi_strerror(error_code);
        (void)error_str; /* Suppress unused warning */
    }

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global curl environment */
    curl_global_cleanup();

    /* Remove dummy file */
    remove_dummy_file();

    return 0;
}