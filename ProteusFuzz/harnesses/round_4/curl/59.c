// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback functions to satisfy type requirements */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_OK;
}

static void dummy_free_callback(void *ptr) {
    (void)ptr;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize libcurl globally */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare data for testing */
    char url_buf[256];
    size_t copy_len = (Size < 255) ? Size : 255;
    memcpy(url_buf, Data, copy_len);
    url_buf[copy_len] = '\0';

    /* Set URL from input data */
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);

    /* Test curl_easy_setopt with various callback options to exercise type checks */
    /* Note: Passing dummy_read_callback (wrong type) to these options tests internal handling */
    curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_read_callback);

    /* Test CURLOPT_ERRORBUFFER */
    char error_buf[CURL_ERROR_SIZE];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);

    /* Test CURLOPT_POSTFIELDS */
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
    }

    /* Test curl_easy_getinfo with various types */
    char *str_info = NULL;
    long long_info = 0;
    double double_info = 0.0;
    struct curl_slist *slist_info = NULL;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;
    curl_socket_t socket_info = -1;
    curl_off_t off_t_info = 0;

    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_info);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_info);

    /* Test curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Test curl_mime_data_cb */
    /* Since curl_mimepart is opaque, we cannot allocate it manually. 
       We create a mime structure and a part using public APIs to get a valid handle. */
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            curl_off_t data_size = (Size > 2) ? (curl_off_t)Data[2] : 0;
            curl_mime_data_cb(part, data_size, dummy_read_callback, dummy_seek_callback, dummy_free_callback, (void *)Data);
        }
        curl_mime_free(mime);
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}