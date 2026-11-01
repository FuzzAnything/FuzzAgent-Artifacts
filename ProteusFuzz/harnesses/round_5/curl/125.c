// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize variables for getinfo
    double d_val;
    long l_val;
    curl_off_t off_val;
    curl_socket_t sock_val;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlsinfo = NULL;
    struct curl_slist *slist = NULL;
    char *str_val = NULL;
    char error_buffer[CURL_ERROR_SIZE];

    // Use input data to set options diversely
    // Set URL
    if (Size > 10) {
        char url[256];
        size_t url_len = (Size < 255) ? Size : 255;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url);
    }

    // Set Error Buffer (tests Wcurl_easy_setopt_err_error_buffer logic)
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    // Set POST Fields (tests Wcurl_easy_setopt_err_postfields logic)
    if (Size > 5) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size);
    }

    // Set Headers (tests Wcurl_easy_setopt_err_curl_slist logic)
    struct curl_slist *headers = NULL;
    if (Size > 10) {
        char header[64];
        size_t h_len = (Size < 63) ? Size : 63;
        memcpy(header, Data, h_len);
        header[h_len] = '\0';
        headers = curl_slist_append(headers, header);
    }
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    // Set Callbacks
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);

    // Perform a dummy request (will likely fail due to invalid URL, but exercises state)
    (void)curl_easy_perform(curl);

    // Test getinfo with various types to trigger type check macros
    // Double type (Wcurl_easy_getinfo_err_double)
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &d_val);

    // Long type
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &l_val);
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &l_val);

    // Off_t type
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_val);

    // Socket type
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_val);

    // Certinfo type (Wcurl_easy_getinfo_err_curl_certinfo)
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    if (certinfo) {
        // Cleanup if needed, though curl_free_certinfo is usually required
        // curl_free_certinfo(&certinfo);
    }

    // TLS Session Info type
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlsinfo);

    // Slist type
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist);
    if (slist) {
        curl_slist_free_all(slist);
    }

    // String type
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_val);

    // Cleanup
    if (headers) {
        curl_slist_free_all(headers);
    }

    curl_easy_cleanup(curl);

    return 0;
}