// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

static void *safe_strdup(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    void *buf = malloc(size + 1);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    ((char *)buf)[size] = '\0';
    return buf;
}

/* 
 * curl_ssls_export_cb signature:
 * CURLcode (*curl_ssls_export_cb)(void *session, void *userptr,
 *                                 const char *protocol,
 *                                 const unsigned char *session_id,
 *                                 unsigned long session_id_len,
 *                                 const unsigned char *session_key,
 *                                 unsigned long session_key_len,
 *                                 long session_version,
 *                                 int session_type,
 *                                 const char *cipher_name,
 *                                 unsigned long session_key_len);
 */
static CURLcode ssls_export_callback(void *session, void *userptr,
                                     const char *protocol,
                                     const unsigned char *session_id,
                                     unsigned long session_id_len,
                                     const unsigned char *session_key,
                                     unsigned long session_key_len,
                                     long session_version,
                                     int session_type,
                                     const char *cipher_name,
                                     unsigned long session_key_len2) {
    (void)session;
    (void)userptr;
    (void)protocol;
    (void)session_id;
    (void)session_id_len;
    (void)session_key;
    (void)session_key_len;
    (void)session_version;
    (void)session_type;
    (void)cipher_name;
    (void)session_key_len2;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *handle = curl_easy_init();
    if (!handle) return 0;

    // Initialize global trace if input suggests it (first byte as flag)
    if (Data[0] & 0x01) {
        char *config_str = safe_strdup(Data + 1, Size > 1 ? Size - 1 : 0);
        if (config_str) {
            curl_global_trace(config_str);
            free(config_str);
        }
    }

    // Set options based on input data
    // Use different parts of the buffer for different options to explore states
    size_t offset = 0;
    size_t chunk_size = Size / 4;

    // CURLOPT_URL
    if (chunk_size > 0) {
        char *url = safe_strdup(Data + offset, chunk_size);
        if (url) {
            curl_easy_setopt(handle, CURLOPT_URL, url);
            free(url);
        }
        offset += chunk_size;
    }

    // CURLOPT_USERAGENT
    if (offset + chunk_size <= Size && chunk_size > 0) {
        char *ua = safe_strdup(Data + offset, chunk_size);
        if (ua) {
            curl_easy_setopt(handle, CURLOPT_USERAGENT, ua);
            free(ua);
        }
        offset += chunk_size;
    }

    // CURLOPT_POSTFIELDS
    // Note: curl_easy_setopt with CURLOPT_POSTFIELDS stores the pointer.
    // The memory must remain valid until curl_easy_cleanup is called.
    // Therefore, we do NOT free 'post' here.
    if (offset + chunk_size <= Size && chunk_size > 0) {
        char *post = safe_strdup(Data + offset, chunk_size);
        if (post) {
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post);
            // Removed: free(post);
        }
        offset += chunk_size;
    }

    // CURLOPT_PROXY
    if (offset + chunk_size <= Size && chunk_size > 0) {
        char *proxy = safe_strdup(Data + offset, chunk_size);
        if (proxy) {
            curl_easy_setopt(handle, CURLOPT_PROXY, proxy);
            free(proxy);
        }
    }

    // Perform the transfer
    curl_easy_perform(handle);

    // Retrieve info
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    struct curl_certinfo certinfo;
    memset(&certinfo, 0, sizeof(certinfo));
    curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo);

    // Try SSL export
    curl_easy_ssls_export(handle, ssls_export_callback, NULL);

    // Perform upkeep
    curl_easy_upkeep(handle);

    // curl_easy_cleanup will handle the cleanup of the handle.
    // Note: curl does not automatically free the memory for CURLOPT_POSTFIELDS
    // in older versions or specific configurations, but in modern libcurl,
    // it often copies the string or manages it. However, to be safe and 
    // consistent with the crash scenario where it didn't copy, the standard
    // practice for fuzzing is to let curl cleanup or ensure the memory is valid.
    // If curl copies the string (CURLOPT_POSTFIELDS usually does in newer versions),
    // the free is safe. If it doesn't, the crash happens.
    // The crash report indicates curl accessed the pointer after we freed it.
    // This implies curl did NOT copy the string in this specific build/version.
    // Thus, we must NOT free it before cleanup.
    curl_easy_cleanup(handle);
    
    return 0;
}