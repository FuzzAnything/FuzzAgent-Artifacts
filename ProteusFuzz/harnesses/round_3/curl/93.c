// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

static void *ssl_export_userptr;
static int ssl_export_called;

/* 
 * The curl_ssls_export_cb signature is:
 * CURLcode callback(void *userptr, void *handle, const char *session_id,
 *                   const unsigned char *session, size_t session_len,
 *                   const unsigned char *cert, size_t cert_len,
 *                   long cert_type, int cert_flags, const char *cipher,
 *                   size_t cipher_len);
 */
static CURLcode ssl_export_callback(void *userptr, void *handle, 
                                    const char *session_id,
                                    const unsigned char *session, size_t session_len,
                                    const unsigned char *cert, size_t cert_len,
                                    long cert_type, int cert_flags, 
                                    const char *cipher, size_t cipher_len) {
    (void)userptr;
    (void)handle;
    (void)session_id;
    (void)session;
    (void)session_len;
    (void)cert;
    (void)cert_len;
    (void)cert_type;
    (void)cert_flags;
    (void)cipher;
    (void)cipher_len;
    ssl_export_called = 1;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl globally once
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    // Fuzz curl_global_trace with input data as config string
    if (Size > 0) {
        char *config_str = malloc(Size + 1);
        if (config_str) {
            memcpy(config_str, Data, Size);
            config_str[Size] = '\0';
            curl_global_trace(config_str);
            free(config_str);
        }
    }

    // Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Set a dummy URL to allow perform to run (though it will likely fail without network)
    if (Size > 10) {
        char url[256];
        size_t url_len = Size < 255 ? Size : 255;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        // Ensure it looks like a URL to avoid immediate validation errors if possible
        if (strncmp(url, "http", 4) != 0 && strncmp(url, "ftp", 3) != 0) {
            // Prefix with http:// if not present to make it a valid URL string
            char full_url[300];
            snprintf(full_url, sizeof(full_url), "http://%s", url);
            curl_easy_setopt(curl, CURLOPT_URL, full_url);
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, url);
        }
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    // Set timeout to avoid hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // Create a MIME structure for fuzzing curl_mime_filename
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Fuzz curl_mime_filename with input data as filename
            if (Size > 0) {
                char *filename = malloc(Size + 1);
                if (filename) {
                    memcpy(filename, Data, Size);
                    filename[Size] = '\0';
                    curl_mime_filename(part, filename);
                    free(filename);
                }
            } else {
                curl_mime_filename(part, "dummy_file");
            }
        }
        curl_mime_free(mime);
    }

    // Perform the transfer
    curl_easy_perform(curl);

    // Fuzz curl_easy_getinfo with various CURLINFO options
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    long size_download = 0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);

    char *effective_url = NULL;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    if (effective_url) {
        // No need to free, curl manages this
    }

    // Fuzz curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Fuzz curl_easy_ssls_export
    ssl_export_called = 0;
    ssl_export_userptr = (void *)(long)Data; // Use input as dummy userptr
    curl_easy_ssls_export(curl, ssl_export_callback, ssl_export_userptr);

    // Cleanup
    curl_easy_cleanup(curl);

    return 0;
}