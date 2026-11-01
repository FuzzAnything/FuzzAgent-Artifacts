// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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

/* 
 * The curl_ssls_export_cb signature is complex and varies by curl version.
 * We define a static callback matching the expected signature for recent curl versions.
 */
static CURLcode dummy_ssls_export_cb(void *userptr, void *curl, 
                                     const char *id, const unsigned char *id_data, 
                                     unsigned long id_len, 
                                     const unsigned char *cert_data, unsigned long cert_len,
                                     long cert_type, int cert_flags, 
                                     const char *cert_name, unsigned long cert_name_len) {
    (void)userptr;
    (void)curl;
    (void)id;
    (void)id_data;
    (void)id_len;
    (void)cert_data;
    (void)cert_len;
    (void)cert_type;
    (void)cert_flags;
    (void)cert_name;
    (void)cert_name_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize global trace if input suggests it (first byte)
    if (Data[0] & 0x01) {
        size_t config_len = (Size > 10) ? 10 : Size;
        char *config = malloc(config_len + 1);
        if (config) {
            memcpy(config, Data, config_len);
            config[config_len] = '\0';
            curl_global_trace(config);
            free(config);
        }
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a simple URL to ensure perform has something to do
    size_t url_len = (Size > 100) ? 100 : Size;
    char *url = malloc(url_len + 1);
    if (!url) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url, Data, url_len);
    url[url_len] = '\0';

    // Basic setup
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);

    // Fuzz curl_easy_perform
    if (Data[0] & 0x02) {
        curl_easy_perform(curl);
    }

    // Fuzz curl_easy_getinfo with various types based on input
    if (Data[0] & 0x04) {
        long long_val = 0;
        double double_val = 0.0;
        char *str_val = NULL;
        struct curl_slist *list_val = NULL;
        struct curl_blob *blob_val = NULL;
        struct curl_version_info_data *version_val = NULL;

        // Select info type based on input byte 1, clamped to valid range
        // Check if we have at least 2 bytes before accessing Data[1]
        CURLINFO info_type;
        if (Size > 1) {
            info_type = (CURLINFO)(Data[1] % 120); 
        } else {
            info_type = CURLINFO_NONE;
        }

        // Attempt to get info based on type guessing
        // We try multiple types to trigger different code paths in the variadic function
        curl_easy_getinfo(curl, info_type, &long_val);
        curl_easy_getinfo(curl, info_type, &double_val);
        curl_easy_getinfo(curl, info_type, &str_val);
        curl_easy_getinfo(curl, info_type, &list_val);
        curl_easy_getinfo(curl, info_type, &blob_val);
        curl_easy_getinfo(curl, info_type, &version_val);
    }

    // Fuzz curl_easy_pause
    if (Data[0] & 0x08) {
        int bitmask;
        if (Size > 2) {
            bitmask = (int)(Data[2] & 0x03); // CURLPAUSE_RECV (1), CURLPAUSE_SEND (2), CURLPAUSE_ALL (3)
        } else {
            bitmask = 0;
        }
        curl_easy_pause(curl, bitmask);
        // Also try unpause
        curl_easy_pause(curl, CURLPAUSE_CONT);
    }

    // Fuzz curl_easy_ssls_export
    if (Data[0] & 0x10) {
        void *userptr = NULL;
        if (Size > 3) {
            userptr = (void*)(size_t)(Data[3]);
        }
        curl_easy_ssls_export(curl, dummy_ssls_export_cb, userptr);
    }

    // Fuzz curl_easy_upkeep
    if (Data[0] & 0x20) {
        curl_easy_upkeep(curl);
    }

    // Cleanup
    curl_easy_cleanup(curl);
    free(url);
    curl_global_cleanup();

    return 0;
}