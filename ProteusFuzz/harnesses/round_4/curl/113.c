// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
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

static int dummy_ssh_key_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)clientp;
    return CURLKHSTAT_FINE;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize variables for getinfo
    long long_info = 0;
    double double_info = 0.0;
    curl_socket_t socket_info = CURL_SOCKET_BAD;
    curl_off_t off_t_info = 0;
    char *string_info = NULL;
    struct curl_slist *slist_info = NULL;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlssessioninfo = NULL;

    // Ensure input is null-terminated for string operations
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Fuzz CURLOPT_URL (String option)
    curl_easy_setopt(curl, CURLOPT_URL, input_str);

    // Fuzz CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)input_str);

    // Fuzz CURLOPT_READFUNCTION and CURLOPT_READDATA
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)input_str);

    // Fuzz CURLOPT_DEBUGFUNCTION and CURLOPT_DEBUGDATA
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)input_str);

    // Fuzz SSH Key Callbacks (Wcurl_easy_setopt_err_ssh_key_cb)
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, (void *)input_str);

    // Fuzz SSH Host Key Callbacks
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void *)input_str);

    // Fuzz Long Options (Wcurl_easy_getinfo_err_long)
    long long_opt = (long)(Size % 1000);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_opt);

    // Fuzz String Options
    curl_easy_setopt(curl, CURLOPT_USERAGENT, input_str);
    curl_easy_setopt(curl, CURLOPT_REFERER, input_str);

    // Perform a dummy perform to trigger state changes (will fail but exercises code)
    curl_easy_perform(curl);

    // Fuzz curl_easy_getinfo with various types
    // Long info
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    // Double info
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
    // Socket info (Wcurl_easy_getinfo_err_curl_socket)
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
    // Off_t info
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_info);
    // String info
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_info);
    // Slist info
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
    // Certinfo (Wcurl_easy_getinfo_err_curl_certinfo)
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    // TlsSessionInfo
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssessioninfo);

    // Cleanup
    if (slist_info) {
        curl_slist_free_all(slist_info);
    }
    // Note: curl_certinfo and curl_tlssessioninfo are freed by curl_easy_cleanup or specific functions
    // depending on version, but usually curl_easy_cleanup handles the handle.
    // We don't free string_info as it's owned by curl.

    curl_easy_cleanup(curl);
    free(input_str);

    return 0;
}