// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_version_info at version.c:596:25 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Static helper to ensure string termination for fuzzing inputs */
static char *create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = (char *)malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = (char *)malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

/* Static dummy callback for CURLOPT_CHUNK_END_FUNCTION */
static int dummy_chunk_end_cb(void *userptr) {
    (void)userptr;
    return 0;
}

/* Static dummy callback for CURLOPT_PROGRESSFUNCTION */
static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

/* Static dummy callback for CURLOPT_SEEKFUNCTION */
static int dummy_seek_cb(void *inclientp, curl_off_t offset, int origin) {
    (void)inclientp;
    (void)offset;
    (void)origin;
    return 0;
}

/* Static dummy callback for CURLOPT_WRITEFUNCTION */
static size_t dummy_write_cb(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Prepare data buffers */
    char *url_buf = create_null_terminated_string(Data, Size);
    char *useragent_buf = create_null_terminated_string(Data, Size);
    char *post_buf = create_null_terminated_string(Data, Size);
    char *range_buf = create_null_terminated_string(Data, Size);

    /* Fuzz curl_version_info with different stamps */
    CURLversion stamp = (CURLversion)(Data[0] % 13);
    curl_version_info_data *ver_info = curl_version_info(stamp);
    if (ver_info) {
        (void)ver_info->version;
        (void)ver_info->version_num;
    }

    /* Fuzz curl_easy_setopt with various options */
    
    /* String options */
    if (url_buf) {
        curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    }
    if (useragent_buf) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent_buf);
    }
    if (range_buf) {
        curl_easy_setopt(curl, CURLOPT_RANGE, range_buf);
    }

    /* Long options (using fuzz data modulo with bounds check) */
    long timeout = (long)(Data[0] % 60);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

    if (Size > 1) {
        long connect_timeout = (long)(Data[1] % 30);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    }

    /* Callback options - corrected names */
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);

    /* Data pointers for callbacks */
    curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

    /* Post fields */
    if (post_buf) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf);
    }

    /* Error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* Fuzz curl_easy_getinfo with various info types */
    char *str_ptr = NULL;
    long long_ptr = 0;
    double double_ptr = 0.0;
    curl_socket_t socket_ptr = CURL_SOCKET_BAD;
    curl_off_t off_t_ptr = 0;

    /* Try to get various info types */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_ptr);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_ptr);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_ptr);
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_ptr);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_ptr);

    /* Cleanup */
    curl_easy_cleanup(curl);
    free(url_buf);
    free(useragent_buf);
    free(post_buf);
    free(range_buf);

    return 0;
}