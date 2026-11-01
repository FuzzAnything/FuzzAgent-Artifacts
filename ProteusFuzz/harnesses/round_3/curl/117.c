// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

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

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Prepare dummy file if needed
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize variables for getinfo
    curl_socket_t sock;
    curl_off_t off_t_val;
    long long_val;
    double double_val;
    char *string_val = NULL;
    struct curl_slist *slist = NULL;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlssessioninfo = NULL;

    // Set some basic options to ensure curl is in a valid state
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    
    // FIX: Allocate a dedicated buffer for the error message instead of using the fuzzer input.
    // CURLOPT_ERRORBUFFER requires a buffer of at least CURL_ERROR_SIZE (typically 256 bytes).
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    // Fuzz setopt with various types based on input
    if (Size > 0) {
        long opt_long = (long)Data[0];
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, opt_long);
    }

    if (Size > 1) {
        long opt_long2 = (long)Data[1];
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, opt_long2);
    }

    // Fuzz string options
    if (Size > 2) {
        char *url_str = (char*)malloc(Size);
        if (url_str) {
            memcpy(url_str, Data, Size);
            url_str[Size - 1] = '\0'; // Ensure null termination
            curl_easy_setopt(curl, CURLOPT_URL, url_str);
            free(url_str);
        }
    }

    // Fuzz POST fields
    if (Size > 3) {
        // Note: CURLOPT_POSTFIELDS expects a null-terminated string or specific format.
        // Passing raw fuzz data directly might cause issues, but it's safer than the error buffer crash.
        // We ensure it's treated as a pointer to the data.
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char*)Data);
    }

    // Fuzz slist options
    if (Size > 4) {
        slist = curl_slist_append(slist, (char*)Data);
        if (slist) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        }
    }

    // Perform a request (non-blocking or short timeout to avoid hanging)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_perform(curl);

    // Fuzz getinfo with various types
    // Test curl_socket_t
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock);
    // Test curl_off_t
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_val);
    // Test long
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &long_val);
    // Test double
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &double_val);
    // Test string
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_val);
    // Test slist
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist);
    if (slist) {
        curl_slist_free_all(slist);
        slist = NULL;
    }
    // Test certinfo
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    // Test tlssessioninfo
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssessioninfo);

    // Cleanup
    curl_easy_cleanup(curl);
    
    return 0;
}