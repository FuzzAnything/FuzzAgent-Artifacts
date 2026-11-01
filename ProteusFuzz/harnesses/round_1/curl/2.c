// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/*
 * The curl_ssls_export_cb type signature is complex and varies by curl version.
 * Based on the error message, the expected signature is:
 * CURLcode (*)(void *, void *, const char *, const unsigned char *, size_t,
 *              const unsigned char *, size_t, long, int, const char *, size_t)
 *
 * We define a static callback matching this signature to avoid compilation errors.
 */
static CURLcode ssls_export_callback(void *userdata, void *session,
                                     const char *id, const unsigned char *id_data,
                                     size_t id_len, const unsigned char *session_data,
                                     size_t session_len, long age, int is_resumed,
                                     const char *protocol, size_t protocol_len) {
    (void)userdata;
    (void)session;
    (void)id;
    (void)id_data;
    (void)id_len;
    (void)session_data;
    (void)session_len;
    (void)age;
    (void)is_resumed;
    (void)protocol;
    (void)protocol_len;
    return CURLE_OK;
}

static void cleanup_handle(CURL *handle) {
    if (handle) {
        curl_easy_cleanup(handle);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *handle = NULL;
    long http_code = 0;

    if (Size == 0) {
        return 0;
    }

    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    // Use a dummy URL to avoid network dependency
    const char *dummy_url = "file:///./dummy_file";
    res = curl_easy_setopt(handle, CURLOPT_URL, dummy_url);
    if (res != CURLE_OK) {
        cleanup_handle(handle);
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy POST field if data is available
    if (Size > 0) {
        res = curl_easy_setopt(handle, CURLOPT_POSTFIELDS, (const char *)Data);
        if (res == CURLE_OK) {
            res = curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (long)Size);
        }
    }

    // Test curl_easy_perform
    res = curl_easy_perform(handle);
    (void)res;

    // Test curl_easy_getinfo
    res = curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &http_code);
    (void)res;

    // Test curl_easy_upkeep
    res = curl_easy_upkeep(handle);
    (void)res;

    // Test curl_easy_ssls_export
    res = curl_easy_ssls_export(handle, ssls_export_callback, NULL);
    (void)res;

    // Cleanup handle
    cleanup_handle(handle);

    // Test curl_global_trace with fuzz data as config string
    if (Size > 0 && Data[Size - 1] != 0) {
        char *term_data = malloc(Size + 1);
        if (term_data) {
            memcpy(term_data, Data, Size);
            term_data[Size] = '\0';
            res = curl_global_trace(term_data);
            (void)res;
            free(term_data);
        }
    } else if (Size > 0) {
        res = curl_global_trace((const char *)Data);
        (void)res;
    }

    curl_global_cleanup();
    return 0;
}