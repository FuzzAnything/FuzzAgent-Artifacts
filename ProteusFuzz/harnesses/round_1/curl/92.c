// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
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
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl API functions:
 * - curl_easy_ssls_export
 * - curl_easy_getinfo
 * - curl_easy_perform
 * - curl_global_trace
 * - curl_easy_upkeep
 */

/* Global buffer to ensure data remains valid during curl operations */
static char g_read_buffer[4096];
static size_t g_read_offset = 0;

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)userdata;
    size_t n = size * nmemb;
    if (n > sizeof(g_read_buffer) - g_read_offset) {
        n = sizeof(g_read_buffer) - g_read_offset;
    }
    if (n == 0) {
        return 0;
    }
    memcpy(ptr, g_read_buffer + g_read_offset, n);
    g_read_offset += n;
    return n;
}

/*
 * Correct signature for curl_ssls_export_cb
 */
static CURLcode ssls_export_callback(void *userdata, void *session,
                                     const char *session_id, const unsigned char *session_id_data,
                                     unsigned long session_id_len,
                                     const unsigned char *session_data, unsigned long session_data_len,
                                     long session_type, int session_protocol,
                                     const char *session_protocol_str, unsigned long session_protocol_len) {
    (void)userdata;
    (void)session;
    (void)session_id;
    (void)session_id_data;
    (void)session_id_len;
    (void)session_data;
    (void)session_data_len;
    (void)session_type;
    (void)session_protocol;
    (void)session_protocol_str;
    (void)session_protocol_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a dummy file for potential file-based options */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Set a safe default URL to avoid immediate failures */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");

    /* Set write and read callbacks to avoid memory issues */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(handle, CURLOPT_READDATA, NULL); /* Not used with read_callback directly but good practice */

    /* Prepare global read buffer with fuzz data to ensure it stays in scope */
    size_t copy_len = (Size < sizeof(g_read_buffer)) ? Size : sizeof(g_read_buffer);
    memcpy(g_read_buffer, Data, copy_len);
    g_read_offset = 0;

    /* Use fuzz data to configure options safely */
    if (Size > 0) {
        /* Use first byte to decide if we set a custom URL */
        if (Data[0] % 2 == 0) {
            char url[256];
            size_t url_len = (Size < 255) ? Size : 255;
            memcpy(url, Data, url_len);
            url[url_len] = '\0';
            curl_easy_setopt(handle, CURLOPT_URL, url);
        }

        /* Use second byte to decide if we set POST data */
        if (Size > 1 && Data[1] % 2 == 0) {
            /* Use a static buffer for POST data to ensure it lives long enough */
            static char post_data[256];
            size_t post_len = (Size < 255) ? Size : 255;
            memcpy(post_data, Data, post_len);
            post_data[post_len] = '\0';
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data);
            curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, post_len);
        }

        /* Use third byte to decide if we set a custom interface */
        if (Size > 2) {
            char iface[64];
            size_t iface_len = (Size < 63) ? Size : 63;
            memcpy(iface, Data, iface_len);
            iface[iface_len] = '\0';
            curl_easy_setopt(handle, CURLOPT_INTERFACE, iface);
        }
    }

    /* Call curl_global_trace with fuzz data as config string */
    if (Size > 0) {
        char config[256];
        size_t config_len = (Size < 255) ? Size : 255;
        memcpy(config, Data, config_len);
        config[config_len] = '\0';
        curl_global_trace(config);
    }

    /* Perform the transfer */
    res = curl_easy_perform(handle);
    (void)res;

    /* Call curl_easy_upkeep */
    res = curl_easy_upkeep(handle);
    (void)res;

    /* Call curl_easy_getinfo with various info types */
    long long_val;
    double double_val;
    curl_socket_t socket_val;
    curl_off_t off_val;
    char *string_val = NULL;
    struct curl_slist *slist_val = NULL;

    /* Test various CURLINFO options */
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &long_val);
    curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &off_val);
    curl_easy_getinfo(handle, CURLINFO_SIZE_UPLOAD, &off_val);
    curl_easy_getinfo(handle, CURLINFO_SPEED_DOWNLOAD, &double_val);
    curl_easy_getinfo(handle, CURLINFO_SPEED_UPLOAD, &double_val);
    curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &string_val);
    if (string_val) {
        string_val = NULL;
    }
    curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &slist_val);
    if (slist_val) {
        curl_slist_free_all(slist_val);
        slist_val = NULL;
    }
    curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &socket_val);

    /* Call curl_easy_ssls_export if available */
    res = curl_easy_ssls_export(handle, ssls_export_callback, NULL);
    (void)res;

    /* Cleanup */
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}