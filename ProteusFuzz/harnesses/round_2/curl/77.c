// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

// Correct signature for curl_ssls_export_cb based on the compiler error
// CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
static CURLcode dummy_ssl_export(void *userptr, void *handle, const char *protocol, 
                                 const unsigned char *cert, unsigned long cert_len,
                                 const unsigned char *key, unsigned long key_len,
                                 long cert_type, int cert_flags, const char *peer_cert, unsigned long peer_cert_len) {
    (void)userptr;
    (void)handle;
    (void)protocol;
    (void)cert;
    (void)cert_len;
    (void)key;
    (void)key_len;
    (void)cert_type;
    (void)cert_flags;
    (void)peer_cert;
    (void)peer_cert_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    int bitmask;
    char dummy_file[] = "./dummy_file";
    FILE *fp;
    size_t bytes_written;
    long http_code;
    double total_time;
    char *url_ptr = NULL;
    const char *config_str = NULL;

    // Initialize curl globally if not already done (safe to call multiple times)
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a CURL handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Prepare dummy file if needed
    fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            bytes_written = fwrite(Data, 1, Size, fp);
            (void)bytes_written;
        }
        fclose(fp);
    }

    // Parse input data to drive fuzzing logic
    if (Size > 0) {
        bitmask = (int)Data[0];
    } else {
        bitmask = 0;
    }

    // Set some basic options to make the handle usable
    // Use a simple URL or the dummy file
    if (Size >= 10) {
        // Try to use input as URL if it looks safe, otherwise use dummy file
        if (Data[0] == 'f') {
            res = curl_easy_setopt(curl, CURLOPT_URL, dummy_file);
        } else {
            // Construct a temporary URL string from input (limited length)
            size_t url_len = Size < 100 ? Size : 100;
            url_ptr = malloc(url_len + 1);
            if (url_ptr) {
                memcpy(url_ptr, Data, url_len);
                url_ptr[url_len] = '\0';
                // Ensure it starts with http/https or file to be valid
                if (strncmp(url_ptr, "http", 4) != 0 && 
                    strncmp(url_ptr, "file", 4) != 0) {
                    url_ptr[0] = 'f'; url_ptr[1] = 'i'; url_ptr[2] = 'l'; url_ptr[3] = 'e';
                }
                res = curl_easy_setopt(curl, CURLOPT_URL, url_ptr);
            }
        }
    } else {
        res = curl_easy_setopt(curl, CURLOPT_URL, dummy_file);
    }
    (void)res; // Ignore setopt errors for fuzzing

    // Fuzz curl_easy_perform
    res = curl_easy_perform(curl);
    (void)res;

    // Fuzz curl_easy_getinfo
    res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    (void)res;

    // Fuzz curl_easy_pause
    // Try different bitmasks derived from input
    res = curl_easy_pause(curl, bitmask);
    (void)res;
    // Try specific pause flags
    res = curl_easy_pause(curl, CURLPAUSE_RECV);
    (void)res;
    res = curl_easy_pause(curl, CURLPAUSE_SEND);
    (void)res;
    res = curl_easy_pause(curl, CURLPAUSE_ALL);
    (void)res;
    res = curl_easy_pause(curl, CURLPAUSE_CONT);
    (void)res;

    // Fuzz curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res;

    // Fuzz curl_easy_ssls_export (requires callback)
    res = curl_easy_ssls_export(curl, dummy_ssl_export, (void*)Data);
    (void)res;

    // Fuzz curl_global_trace
    if (Size >= 1) {
        // Create a null-terminated string from input for config
        size_t config_len = Size < 256 ? Size : 256;
        char *config_buf = malloc(config_len + 1);
        if (config_buf) {
            memcpy(config_buf, Data, config_len);
            config_buf[config_len] = '\0';
            config_str = config_buf;
        }
    }
    if (config_str) {
        res = curl_global_trace(config_str);
        (void)res;
        free((void*)config_str);
    }

    // Cleanup
    if (url_ptr) {
        free(url_ptr);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}