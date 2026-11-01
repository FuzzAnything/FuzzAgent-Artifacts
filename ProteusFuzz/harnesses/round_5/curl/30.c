// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>

static CURLcode ssls_export_callback(void *handle, void *session,
                                     const char *session_id,
                                     const unsigned char *session_id_len,
                                     unsigned long session_id_len_val,
                                     const unsigned char *session_data,
                                     unsigned long session_data_len,
                                     long age, int type,
                                     const char *cipher, unsigned long cipher_len) {
    (void)handle;
    (void)session;
    (void)session_id;
    (void)session_id_len;
    (void)session_id_len_val;
    (void)session_data;
    (void)session_data_len;
    (void)age;
    (void)type;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Initialize global trace with a safe default or derived from input if safe
    // Since curl_global_trace expects a config string, we ensure null termination
    char *trace_config = (char *)malloc(Size + 1);
    if (trace_config) {
        memcpy(trace_config, Data, Size);
        trace_config[Size] = '\0';
        curl_global_trace(trace_config);
        free(trace_config);
    }

    // Prepare a null-terminated string for options that require it
    // We must keep this string alive for the duration of curl_easy_perform
    // because curl may store a pointer to it or read it later.
    char *opt_string = (char *)malloc(Size + 1);
    if (!opt_string) {
        curl_easy_cleanup(handle);
        return 0;
    }
    
    memcpy(opt_string, Data, Size);
    opt_string[Size] = '\0';

    // Set various options to explore different states
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(handle, CURLOPT_USERAGENT, opt_string);
    curl_easy_setopt(handle, CURLOPT_PROXY, opt_string);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, opt_string);
    curl_easy_setopt(handle, CURLOPT_VERBOSE, 0L); // Disable verbose to avoid output noise
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);

    // Perform the transfer
    curl_easy_perform(handle);

    // Retrieve information after perform
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    struct curl_certinfo certinfo;
    memset(&certinfo, 0, sizeof(certinfo));
    curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo);

    // Attempt SSL session export
    curl_easy_ssls_export(handle, ssls_export_callback, NULL);

    // Perform upkeep
    curl_easy_upkeep(handle);

    // Cleanup
    curl_easy_cleanup(handle);
    free(opt_string);

    return 0;
}