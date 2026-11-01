// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

/* Dummy callback functions to satisfy type requirements */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return size * nmemb;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

/* Helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Create dummy file for file-based options */
    create_dummy_file();

    /* Use input data to drive fuzzing logic */
    unsigned int seed = Data[0];
    unsigned int option_seed = (Size > 1) ? Data[1] : 0;
    unsigned int info_seed = (Size > 2) ? Data[2] : 0;

    /* Configure easy handle with various options based on input */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/fuzz");
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, (char[256]){0});

    /* Fuzz easy options based on seed */
    if (option_seed % 5 == 0) {
        curl_easy_setopt(easy_handle, CURLOPT_CAINFO, "./dummy_file");
    } else if (option_seed % 5 == 1) {
        curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE, "./dummy_file");
    } else if (option_seed % 5 == 2) {
        curl_easy_setopt(easy_handle, CURLOPT_SSLCERT, "./dummy_file");
    } else if (option_seed % 5 == 3) {
        curl_easy_setopt(easy_handle, CURLOPT_PROXY, "http://proxy.example.com");
    } else {
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, "FuzzAgent");
    }

    /* Add easy handle to multi handle */
    CURLMcode mcode = curl_multi_add_handle(multi_handle, easy_handle);
    (void)mcode; /* Ignore result for fuzzing state exploration */

    /* Fuzz multi options */
    if (seed % 3 == 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10);
    } else if (seed % 3 == 1) {
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_HTTP1);
    } else {
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 5);
    }

    /* Test curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Test curl_multi_strerror with various error codes */
    CURLMcode test_errors[] = {
        CURLM_OK, CURLM_CALL_MULTI_PERFORM, CURLM_BAD_HANDLE,
        CURLM_BAD_EASY_HANDLE, CURLM_OUT_OF_MEMORY, CURLM_INTERNAL_ERROR,
        CURLM_BAD_SOCKET, CURLM_UNKNOWN_OPTION, CURLM_ADDED_ALREADY,
        CURLM_RECURSIVE_API_CALL, CURLM_WAKEUP_FAILURE
    };
    size_t num_errors = sizeof(test_errors) / sizeof(test_errors[0]);
    for (size_t i = 0; i < num_errors; i++) {
        const char *str = curl_multi_strerror(test_errors[i]);
        (void)str; /* Use the string to avoid unused variable warning */
    }

    /* Fuzz curl_easy_getinfo with various info types */
    char *string_info;
    long long_info;
    double double_info;
    curl_off_t off_t_info;
    curl_socket_t socket_info;
    struct curl_slist *slist_info;
    struct curl_certinfo *certinfo;
    struct curl_tlssessioninfo *tlssessioninfo;

    /* Test string info */
    if (info_seed % 4 == 0) {
        curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &string_info);
    }
    /* Test long info */
    else if (info_seed % 4 == 1) {
        curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_info);
    }
    /* Test double info */
    else if (info_seed % 4 == 2) {
        curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &double_info);
    }
    /* Test off_t info */
    else if (info_seed % 4 == 3) {
        curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &off_t_info);
    }

    /* Test socket info */
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &socket_info);

    /* Test slist info */
    curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &slist_info);
    if (slist_info) {
        curl_slist_free_all(slist_info);
    }

    /* Test certinfo */
    curl_easy_getinfo(easy_handle, CURLINFO_CERTINFO, &certinfo);
    /* No cleanup needed for certinfo as curl manages it */

    /* Test tlssessioninfo */
    curl_easy_getinfo(easy_handle, CURLINFO_TLS_SESSION, &tlssessioninfo);
    /* No cleanup needed for tlssessioninfo as curl manages it */

    /* Perform multi perform to exercise more states */
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    /* Clean up dummy file */
    remove("./dummy_file");

    return 0;
}