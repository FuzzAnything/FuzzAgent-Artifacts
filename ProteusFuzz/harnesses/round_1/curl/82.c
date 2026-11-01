// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl library functions.
 * Targets: curl_global_trace, curl_easy_strerror, and various curl_easy_setopt/getinfo
 * scenarios that trigger type-checking macros (Wcurl_*_err_*) at compile time.
 * Since the Wcurl_* macros are compile-time warnings, we fuzz the runtime functions
 * that use them to ensure robustness against invalid inputs and edge cases.
 */

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * 1. Fuzz curl_global_trace
     * The function takes a config string. We use the input data as the config string.
     * Ensure null termination.
     */
    if (Size > 0) {
        char *config_str = (char *)malloc(Size + 1);
        if (config_str) {
            memcpy(config_str, Data, Size);
            config_str[Size] = '\0';
            curl_global_trace(config_str);
            free(config_str);
        }
    }

    /*
     * 2. Fuzz curl_easy_strerror
     * Convert various parts of the input to CURLcode to test error string generation.
     * We cast the input bytes to CURLcode to explore different error states.
     */
    if (Size >= sizeof(CURLcode)) {
        CURLcode error_code = *(const CURLcode *)Data;
        const char *str = curl_easy_strerror(error_code);
        (void)str; /* Use the result to avoid unused variable warnings */
    }

    /*
     * 3. Fuzz curl_easy_setopt with various option types.
     * We attempt to set options that correspond to the type-checking macros mentioned.
     * Note: The Wcurl_* macros are compile-time checks. At runtime, we pass potentially
     * malformed data to see how curl handles it.
     */

    /* Set URL (string option) */
    if (Size > 0) {
        char *url = (char *)malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
            free(url);
        }
    }

    /* Set POSTFIELDS (void* or char*) - targets Wcurl_easy_setopt_err_postfields */
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (void *)Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
    }

    /* Set a long option (e.g., CURLOPT_TIMEOUT) */
    if (Size >= sizeof(long)) {
        long timeout = *(const long *)Data;
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }

    /* Set an off_t option (e.g., CURLOPT_MAXFILESIZE_LARGE) */
    if (Size >= sizeof(curl_off_t)) {
        curl_off_t max_size = *(const curl_off_t *)Data;
        curl_easy_setopt(curl, CURLOPT_MAXFILESIZE_LARGE, max_size);
    }

    /* Set a callback option (e.g., CURLOPT_WRITEFUNCTION) */
    /* We pass NULL or a dummy pointer to test error handling */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (curl_write_callback)Data);

    /* Set SSL_CTX_DATA (targets Wcurl_easy_setopt_err_ssl_ctx_cb context) */
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, (void *)Data);

    /*
     * 4. Fuzz curl_easy_getinfo with various info types.
     * We use dummy variables to receive the info.
     */
    char *dummy_str = NULL;
    long dummy_long = 0;
    double dummy_double = 0.0;
    curl_off_t dummy_off_t = 0;
    curl_socket_t dummy_socket = 0;
    struct curl_slist *dummy_slist = NULL;
    struct curl_tlssessioninfo dummy_tls = {0};
    struct curl_certinfo dummy_cert = {0};

    /* CURLINFO_EFFECTIVE_URL (string) */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &dummy_str);

    /* CURLINFO_RESPONSE_CODE (long) */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &dummy_long);

    /* CURLINFO_SIZE_DOWNLOAD_T (off_t) - targets Wcurl_easy_getinfo_err_curl_off_t */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &dummy_off_t);

    /* CURLINFO_SIZE_UPLOAD_T (off_t) */
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &dummy_off_t);

    /* CURLINFO_HTTP_CONNECTCODE (long) */
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &dummy_long);

    /* CURLINFO_TOTAL_TIME (double) */
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dummy_double);

    /* CURLINFO_TLS_SESSION (struct curl_tlssessioninfo *) - targets Wcurl_easy_getinfo_err_curl_tlssessioninfo */
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &dummy_tls);

    /* CURLINFO_CERTINFO (struct curl_certinfo *) - targets Wcurl_easy_getinfo_err_curl_certinfo */
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &dummy_cert);

    /* CURLINFO_COOKIELIST (struct curl_slist *) - targets Wcurl_easy_getinfo_err_curl_slist */
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &dummy_slist);
    if (dummy_slist) {
        curl_slist_free_all(dummy_slist);
    }

    /* CURLINFO_PRIVATE (string) */
    curl_easy_getinfo(curl, CURLINFO_PRIVATE, &dummy_str);

    /* CURLINFO_SIZE_DOWNLOAD (double) */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &dummy_double);

    /*
     * 5. Perform a dummy request to exercise more code paths.
     * We use a dummy URL or the input data if it looks like a URL.
     * Fix: Allocate a buffer large enough for CURLOPT_ERRORBUFFER (CURL_ERROR_SIZE).
     * The previous code reused 'test_url' which was Size+1, potentially smaller than CURL_ERROR_SIZE.
     */
    if (Size > 0) {
        char *test_url = (char *)malloc(Size + 1);
        char error_buffer[CURL_ERROR_SIZE];
        
        if (test_url) {
            memcpy(test_url, Data, Size);
            test_url[Size] = '\0';
            
            curl_easy_setopt(curl, CURLOPT_URL, test_url);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 0L);
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
            
            /* Use the dedicated error buffer instead of reusing test_url */
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
            
            res = curl_easy_perform(curl);
            (void)res; /* Ignore result for fuzzing stability */
            
            free(test_url);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}