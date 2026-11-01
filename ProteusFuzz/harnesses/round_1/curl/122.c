// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize a dummy file for file-based options if needed
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Use input data to fuzz various options
    char *url = NULL;
    char *header = NULL;
    struct curl_slist *headers = NULL;
    long long_val;
    double double_val;
    char *string_val = NULL;
    curl_off_t off_t_val;
    curl_socket_t socket_val;
    struct curl_tlssessioninfo tls_info;
    struct curl_certinfo cert_info;
    char error_buffer[CURL_ERROR_SIZE];

    // Set URL from input if it looks like a valid URL prefix
    if (Size > 4 && (Data[0] == 'h' || Data[0] == 'f' || Data[0] == 'd')) {
        url = (char *)malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
        }
    } else {
        // Fallback to a dummy URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    // Fuzz CURLOPT_HTTPHEADER with curl_slist
    if (Size > 10) {
        headers = curl_slist_append(headers, (char *)(Data + 10));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    // Fuzz CURLOPT_POSTFIELDS
    if (Size > 20) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)(Data + 20));
    }

    // Fuzz CURLOPT_READDATA and CURLOPT_WRITEDATA
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

    // Fuzz CURLOPT_WRITEFUNCTION and CURLOPT_READFUNCTION
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);

    // Fuzz CURLOPT_ERRORBUFFER
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    // Fuzz CURLOPT_VERBOSE (long)
    long_val = (Size > 0) ? (long)Data[0] : 1;
    curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);

    // Fuzz CURLOPT_TIMEOUT (long)
    long_val = (Size > 1) ? (long)Data[1] : 30;
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);

    // Fuzz CURLOPT_INFILESIZE_LARGE (curl_off_t)
    off_t_val = (Size > 2) ? (curl_off_t)Data[2] : 0;
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, off_t_val);

    // Perform the request (non-blocking or with timeout to avoid hanging)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100);
    curl_easy_perform(curl);

    // Fuzz curl_easy_getinfo with various info types
    // CURLINFO_RESPONSE_CODE (long)
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    // CURLINFO_SIZE_DOWNLOAD_T (double)
    double download_size;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_size);

    // CURLINFO_EFFECTIVE_URL (char *)
    char *effective_url;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);

    // CURLINFO_COOKIELIST (struct curl_slist *)
    struct curl_slist *cookie_list;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookie_list);
    if (cookie_list) {
        curl_slist_free_all(cookie_list);
    }

    // CURLINFO_TLS_SESSION (struct curl_tlssessioninfo *)
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);

    // CURLINFO_CERTINFO (struct curl_certinfo *)
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);

    // CURLINFO_SIZE_UPLOAD_T (curl_off_t)
    curl_off_t upload_size;
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_size);

    // CURLINFO_ACTIVESOCKET (curl_socket_t)
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_val);

    // Cleanup
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (url) {
        free(url);
    }
    curl_easy_cleanup(curl);

    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}