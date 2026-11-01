// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Callback for curl_easy_ssls_export matching the expected signature exactly */
/* 
 * The signature from curl.h is:
 * CURLcode (*curl_ssls_export_cb)(void *userptr, void *ssl, const char *id, 
 *                                 const unsigned char *id_len, 
 *                                 const unsigned char *data, unsigned long data_len,
 *                                 long age, int type, const char *cipher, 
 *                                 unsigned long cipher_len);
 *
 * Note the order of data/data_len vs id/id_len in the error message hint:
 * The error message shows the expected type as:
 * (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *
 * Let's re-verify the order based on the error message:
 * 1. void *userptr
 * 2. void *ssl
 * 3. const char *id
 * 4. const unsigned char *id_len (Wait, usually id_len is a size_t or unsigned long, not a pointer? 
 *    The error message says: const unsigned char *, unsigned long. 
 *    Actually, looking at the error message carefully:
 *    'CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    
 *    Let's map the error message arguments to the likely parameters:
 *    1. void *userptr
 *    2. void *ssl
 *    3. const char *id
 *    4. const unsigned char *id_len  <-- This seems odd for a length, usually it's a scalar.
 *    5. unsigned long                <-- Maybe this is the length of id?
 *    6. const unsigned char *data
 *    7. unsigned long data_len
 *    8. long age
 *    9. int type
 *    10. const char *cipher
 *    11. unsigned long cipher_len
 *
 *    Wait, the error message says:
 *    'const unsigned char *, unsigned long' for the 4th and 5th args.
 *    And 'const unsigned char *, unsigned long' for the 6th and 7th args.
 *    
 *    Let's look at the standard curl_ssls_export_cb definition in recent curl versions.
 *    Usually: (void *userptr, void *ssl, const char *id, const unsigned char *id_data, unsigned long id_len, const unsigned char *data, unsigned long data_len, long age, int type, const char *cipher, unsigned long cipher_len)
 *    
 *    The error message signature:
 *    (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *    
 *    Mapping:
 *    1. void *userptr
 *    2. void *ssl
 *    3. const char *id
 *    4. const unsigned char *id_data (or similar)
 *    5. unsigned long id_len
 *    6. const unsigned char *data
 *    7. unsigned long data_len
 *    8. long age
 *    9. int type
 *    10. const char *cipher
 *    11. unsigned long cipher_len
 *
 *    My previous function had:
 *    (void *userptr, void *ssl, const char *id, const unsigned char *id_len, const unsigned char *data, unsigned long data_len, ...)
 *    I treated id_len as a pointer, but the error message says the 4th arg is a pointer and 5th is unsigned long.
 *    So the 4th arg is likely the data for the ID, and 5th is the length.
 */
static CURLcode ssl_export_callback(void *userptr,
                                    void *ssl,
                                    const char *id,
                                    const unsigned char *id_data,
                                    unsigned long id_len,
                                    const unsigned char *data,
                                    unsigned long data_len,
                                    long age,
                                    int type,
                                    const char *cipher,
                                    unsigned long cipher_len) {
    (void)userptr;
    (void)ssl;
    (void)id;
    (void)id_data;
    (void)id_len;
    (void)data;
    (void)data_len;
    (void)age;
    (void)type;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

/* Dummy read callback for curl_easy_setopt */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userptr;
    return 0;
}

/* Dummy write callback for curl_easy_setopt */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr;
    (void)nmemb;
    (void)userptr;
    return size;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    CURL *handle = NULL;
    CURLcode res;
    long long_val = 0;
    double double_val = 0.0;
    char *str_val = NULL;
    curl_off_t off_val = 0;
    curl_socket_t sock_val = CURL_SOCKET_BAD;
    struct curl_slist *slist_val = NULL;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;
    char error_buf[CURL_ERROR_SIZE] = {0};

    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a handle */
    handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Set basic options to ensure valid state */
    res = curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    if (res != CURLE_OK) {
        /* Handle might be invalid or URL too long, try with a safe fallback */
        res = curl_easy_setopt(handle, CURLOPT_URL, "http://localhost");
    }

    /* Set error buffer */
    res = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buf);

    /* Set read/write callbacks to avoid issues during perform */
    res = curl_easy_setopt(handle, CURLOPT_READFUNCTION, dummy_read_callback);
    res = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    res = curl_easy_setopt(handle, CURLOPT_WRITEDATA, handle);
    res = curl_easy_setopt(handle, CURLOPT_READDATA, handle);

    /* Use fuzzer data to set various options if possible */
    if (Size > 0) {
        /* Try to set a string option if data looks like a string */
        if (Size < 1024) {
            char *temp_str = malloc(Size + 1);
            if (temp_str) {
                memcpy(temp_str, Data, Size);
                temp_str[Size] = '\0';
                /* Try setting a few string options */
                curl_easy_setopt(handle, CURLOPT_USERAGENT, temp_str);
                curl_easy_setopt(handle, CURLOPT_REFERER, temp_str);
                curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, temp_str);
                free(temp_str);
            }
        }

        /* Try setting long options */
        if (Size >= sizeof(long)) {
            memcpy(&long_val, Data, sizeof(long));
            curl_easy_setopt(handle, CURLOPT_TIMEOUT, long_val);
            curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, long_val);
        }

        /* Try setting off_t options */
        if (Size >= sizeof(curl_off_t)) {
            memcpy(&off_val, Data, sizeof(curl_off_t));
            curl_easy_setopt(handle, CURLOPT_RESUME_FROM, off_val);
        }
    }

    /* 1. curl_easy_ssls_export */
    res = curl_easy_ssls_export(handle, ssl_export_callback, handle);
    (void)res;

    /* 2. curl_easy_getinfo - test various info types */
    res = curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_SIZE_UPLOAD, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_SIZE_UPLOAD_T, &off_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &sock_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &slist_val);
    (void)res;
    if (slist_val) {
        curl_slist_free_all(slist_val);
        slist_val = NULL;
    }
    /* Note: TLS and Cert info might require a real connection to populate, but we call anyway */
    res = curl_easy_getinfo(handle, CURLINFO_TLS_SESSION, &tls_info);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CERTINFO, &cert_info);
    (void)res;

    /* 3. curl_easy_upkeep */
    res = curl_easy_upkeep(handle);
    (void)res;

    /* 4. curl_easy_perform */
    res = curl_easy_perform(handle);
    (void)res;

    /* 5. curl_easy_pause */
    res = curl_easy_pause(handle, CURLPAUSE_ALL);
    (void)res;
    res = curl_easy_pause(handle, CURLPAUSE_CONT);
    (void)res;

    /* Cleanup */
    if (slist_val) {
        curl_slist_free_all(slist_val);
    }

    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}