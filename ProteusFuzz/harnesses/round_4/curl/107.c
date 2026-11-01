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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

static size_t dummy_write_cb(void *contents, size_t size, size_t nmemb, void *userptr) {
    (void)contents;
    (void)userptr;
    return size * nmemb;
}

static CURLcode dummy_ssls_export_cb(CURL *easy, void *userptr, const char *session_id, const unsigned char *session_data, unsigned long session_data_len, const unsigned char *cert_data, unsigned long cert_data_len, long cert_len, int cert_type, const char *cert_type_str, unsigned long cert_type_len) {
    (void)easy;
    (void)userptr;
    (void)session_id;
    (void)session_data;
    (void)session_data_len;
    (void)cert_data;
    (void)cert_data_len;
    (void)cert_len;
    (void)cert_type;
    (void)cert_type_str;
    (void)cert_type_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *handle = NULL;
    char dummy_file_path[] = "./dummy_file";
    FILE *fp = fopen(dummy_file_path, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Fuzz curl_global_trace with a null-terminated string derived from input
    // Ensure the string is null-terminated to prevent buffer overflows
    size_t safe_size = Size;
    if (safe_size > 1024) {
        safe_size = 1024;
    }
    char *safe_config = malloc(safe_size + 1);
    if (safe_config) {
        memcpy(safe_config, Data, safe_size);
        safe_config[safe_size] = '\0';
        res = curl_global_trace(safe_config);
        (void)res;
        free(safe_config);
    }

    handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    // Set some options to make the handle more robust for testing
    res = curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    (void)res;
    
    // FIX: Provide a valid write callback and userdata instead of NULL to prevent
    // the library from attempting to write to a NULL file pointer or callback.
    res = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    (void)res;
    res = curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
    (void)res;

    // Fuzz curl_easy_pause with various bitmasks derived from input
    if (Size >= 1) {
        int bitmask = Data[0];
        res = curl_easy_pause(handle, bitmask);
        (void)res;
    }

    // Fuzz curl_easy_upkeep
    res = curl_easy_upkeep(handle);
    (void)res;

    // Fuzz curl_easy_perform
    res = curl_easy_perform(handle);
    (void)res;

    // Fuzz curl_easy_getinfo with various info types
    long long_val;
    double double_val;
    char *str_val;
    struct curl_slist *list_val;
    struct curl_blob *blob_val;

    res = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_HTTP_VERSION, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_SSL_VERIFYRESULT, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_NAMELOOKUP_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CONNECT_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_PRETRANSFER_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_STARTTRANSFER_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_REDIRECT_COUNT, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_REDIRECT_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_PRIMARY_IP, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_PRIMARY_PORT, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_LOCAL_IP, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_LOCAL_PORT, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_OS_ERRNO, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_UPLOAD, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_HTTP_CONNECTCODE, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_HTTPAUTH_AVAIL, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_PROXYAUTH_AVAIL, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_PRIVATE, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &list_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_FTP_ENTRY_PATH, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_LASTSOCKET, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CONDITION_UNMET, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_RTSP_SESSION_ID, &str_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_RTSP_CLIENT_CSEQ, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_RTSP_CSEQ_RECV, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_RTSP_SERVER_CSEQ, &long_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_APPCONNECT_TIME, &double_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_CERTINFO, &list_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_TLS_SESSION, &blob_val);
    (void)res;
    res = curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &long_val);
    (void)res;

    // Fuzz curl_easy_ssls_export
    res = curl_easy_ssls_export(handle, dummy_ssls_export_cb, (void *)Data);
    (void)res;

    // Cleanup
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}