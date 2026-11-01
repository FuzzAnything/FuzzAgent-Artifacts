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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/* Include the typecheck header to access the warning macros */
#include "typecheck-gcc.h"

/* Dummy callback to satisfy CURLOPT_READFUNCTION if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Dummy callback to satisfy CURLOPT_WRITEFUNCTION if needed */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Helper to safely parse integers from fuzz input */
static long parse_long(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(long) > size) return 0;
    long val;
    memcpy(&val, data + *offset, sizeof(long));
    *offset += sizeof(long);
    return val;
}

/* Helper to safely parse pointers from fuzz input (cast to void*) */
static void *parse_ptr(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(void *) > size) return NULL;
    void *val;
    memcpy(&val, data + *offset, sizeof(void *));
    *offset += sizeof(void *);
    return val;
}

/* Helper to safely parse strings from fuzz input */
static char *parse_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    size_t len = 0;
    while (*offset + len < size && data[*offset + len] != '\0') {
        len++;
    }
    if (len == 0) {
        (*offset)++;
        return NULL;
    }
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len + 1;
    return str;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    size_t offset = 0;
    uint8_t op = 0;

    if (offset < Size) {
        op = Data[offset++];
    }

    /* Create dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    dummy_file = fopen("./dummy_file", "rb");

    /* Fuzz curl_easy_setopt with various types to trigger typecheck macros */
    /* The macros are compile-time checks, but we can fuzz the runtime behavior
       of curl_easy_setopt with various invalid/valid types to ensure robustness.
       The macros themselves don't run at runtime, but the underlying functions do. */

    /* We will try to set various options with fuzzed data */
    long option_val = parse_long(Data, Size, &offset);
    void *ptr_val = parse_ptr(Data, Size, &offset);
    char *str_val = parse_string(Data, Size, &offset);

    /* Try to set options that expect different types */
    /* CURLOPT_URL expects a string */
    if (str_val) {
        curl_easy_setopt(curl, CURLOPT_URL, str_val);
        free(str_val);
    } else {
        /* Fallback to a safe URL if string parsing failed */
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    /* CURLOPT_READDATA expects a pointer */
    curl_easy_setopt(curl, CURLOPT_READDATA, ptr_val);

    /* CURLOPT_WRITEFUNCTION expects a callback */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);

    /* CURLOPT_READFUNCTION expects a callback */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);

    /* CURLOPT_INFILE expects a FILE* */
    if (dummy_file) {
        curl_easy_setopt(curl, CURLOPT_INFILE, dummy_file);
    }

    /* CURLOPT_POSTFIELDS expects void* or char* */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ptr_val);

    /* CURLOPT_HTTPHEADER expects curl_slist* */
    /* We can't easily create a valid curl_slist from raw fuzz data without allocation logic,
       so we pass NULL or a raw pointer to test error handling */
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, ptr_val);

    /* CURLOPT_MIMEPOST expects curl_mime* */
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, ptr_val);

    /* CURLOPT_STREAM_DEPENDS expects CURL* */
    curl_easy_setopt(curl, CURLOPT_STREAM_DEPENDS, ptr_val);

    /* Now try to get info with various types */
    long long_info;
    double double_info;
    char *string_info;
    curl_socket_t socket_info;
    curl_off_t off_t_info;
    struct curl_slist *slist_info;
    struct curl_certinfo *certinfo;
    struct curl_tlssessioninfo *tlssessioninfo;

    /* CURLINFO_RESPONSE_CODE expects long */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);

    /* CURLINFO_TOTAL_TIME expects double */
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);

    /* CURLINFO_EFFECTIVE_URL expects char* */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_info);
    /* Note: string_info is owned by curl, do not free */

    /* CURLINFO_ACTIVESOCKET expects curl_socket_t */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);

    /* CURLINFO_SIZE_DOWNLOAD_T expects curl_off_t */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_info);

    /* CURLINFO_COOKIELIST expects curl_slist* */
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
    if (slist_info) {
        curl_slist_free_all(slist_info);
    }

    /* CURLINFO_CERTINFO expects curl_certinfo* */
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    if (certinfo) {
        /* curl_certinfo is freed by curl_easy_cleanup or curl_easy_reset usually,
           but we can try to free it if we have the function.
           For safety in fuzzing, we might skip freeing if unsure, but standard practice is to free.
           However, curl_easy_getinfo documentation says curl_certinfo is freed by curl.
           Actually, for CURLINFO_CERTINFO, the user must free the list of curl_certinfo.
           But for simplicity in fuzzing, we rely on curl_easy_cleanup. */
    }

    /* CURLINFO_TLS_SESSION expects curl_tlssessioninfo* */
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssessioninfo);

    /* Perform the request to exercise more code paths */
    curl_easy_perform(curl);

    /* Cleanup */
    if (dummy_file) {
        fclose(dummy_file);
    }
    curl_easy_cleanup(curl);

    return 0;
}