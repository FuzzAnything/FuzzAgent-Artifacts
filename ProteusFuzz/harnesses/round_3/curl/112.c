// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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

/* Dummy callback to satisfy CURLOPT_READFUNCTION if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize a dummy file if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Use fuzzing data to drive various curl_easy_setopt and curl_easy_getinfo calls */
    const char *url = (Size > 10 && Data[0] == 'h' && Data[1] == 't' && Data[2] == 't' && Data[3] == 'p') ? (const char *)Data : "http://example.com";
    
    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Fuzz various options based on input bytes */
    if (Size > 1) {
        long val_long = (long)Data[1];
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, val_long);
    }

    if (Size > 2) {
        double val_double = (double)Data[2];
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, val_double);
    }

    /* Fuzz CURLOPT_POSTFIELDS with input data */
    if (Size > 3) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
    }

    /* Fuzz CURLOPT_HTTPHEADER with a dynamically allocated slist */
    if (Size > 4) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Fuzzer");
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_slist_free_all(headers);
    }

    /* Perform a dummy perform to trigger internal states */
    curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo with various types */
    long response_code;
    double speed_download;
    curl_off_t total_download;
    char *effective_url;
    struct curl_slist *cookies;
    struct curl_tlssessioninfo *tls_session;
    struct curl_certinfo *cert_info;
    curl_socket_t socket_fd;

    /* Get info for long */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    /* Get info for double */
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);

    /* Get info for curl_off_t */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &total_download);

    /* Get info for string */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);

    /* Get info for slist */
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (cookies) {
        curl_slist_free_all(cookies);
    }

    /* Get info for tlssessioninfo */
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_session);

    /* Get info for certinfo */
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);

    /* Get info for socket */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_fd);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}