// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Callback for SSL session export matching curl_ssls_export_cb signature */
static CURLcode ssl_export_callback(void *handle, void *userptr,
                                    const char *protocol,
                                    const unsigned char *session_id,
                                    unsigned long session_id_len,
                                    const unsigned char *session,
                                    unsigned long session_len,
                                    long age, int is_resumed,
                                    const char *cipher, unsigned long cipher_len) {
    (void)handle;
    (void)userptr;
    (void)protocol;
    (void)session_id;
    (void)session_id_len;
    (void)session;
    (void)session_len;
    (void)age;
    (void)is_resumed;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

/* Write callback to handle data */
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Read callback to provide data */
static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Create a safe, null-terminated URL buffer */
    /* We allocate enough space for the input plus a null terminator */
    char *url_buf = malloc(Size + 1);
    if (!url_buf) {
        curl_easy_cleanup(curl);
        return 0;
    }
    
    memcpy(url_buf, Data, Size);
    url_buf[Size] = '\0';

    /* Ensure the URL looks somewhat valid to avoid immediate rejection */
    /* Only modify if the buffer is large enough to hold the default string */
    if (url_buf[0] != 'h' && url_buf[0] != 'f' && url_buf[0] != 'd') {
        const char *default_url = "http://example.com";
        size_t default_len = strlen(default_url);
        
        if (Size >= default_len) {
            memcpy(url_buf, default_url, default_len + 1);
        } else {
            /* If input is too small, construct a minimal valid URL safely */
            /* We need at least 8 bytes for "http://x" + null */
            if (Size >= 8) {
                url_buf[0] = 'h';
                url_buf[1] = 't';
                url_buf[2] = 't';
                url_buf[3] = 'p';
                url_buf[4] = ':';
                url_buf[5] = '/';
                url_buf[6] = '/';
                url_buf[7] = 'x';
                url_buf[8] = '\0';
            } else {
                /* If even smaller, just use a tiny valid URL */
                const char *tiny_url = "h://x";
                size_t tiny_len = strlen(tiny_url);
                if (Size >= tiny_len) {
                    memcpy(url_buf, tiny_url, tiny_len + 1);
                } else {
                    /* Fallback: just ensure null termination and valid start */
                    url_buf[0] = 'h';
                    url_buf[1] = '\0';
                }
            }
        }
    }

    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);

    /* Set write function and data */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)0x1234);

    /* Set read function and data */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)0x5678);

    /* Set a dummy file for potential file-based options if needed */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Fuzz curl_easy_setopt with various options based on input */
    if (Size > 1) {
        long val_long = (long)(Data[0] << 8 | Data[1]);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, val_long);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, val_long);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, val_long);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, val_long);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, val_long);
    }

    /* Fuzz string options with a safe copy */
    if (Size > 10) {
        char *str_buf = malloc(Size + 1);
        if (str_buf) {
            memcpy(str_buf, Data + 10, Size - 10);
            str_buf[Size - 10] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
            curl_easy_setopt(curl, CURLOPT_REFERER, str_buf);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, str_buf);
            free(str_buf);
        }
    }

    /* Fuzz POST fields if size allows */
    if (Size > 20) {
        char *post_buf = malloc(Size - 20 + 1);
        if (post_buf) {
            memcpy(post_buf, Data + 20, Size - 20);
            post_buf[Size - 20] = '\0';
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf);
            /* Keep post_buf alive until perform is done */
            curl_easy_perform(curl);
            free(post_buf);
        }
    } else {
        /* Perform without POST data if not enough input */
        curl_easy_perform(curl);
    }

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Fuzz curl_easy_ssls_export if supported */
    curl_easy_ssls_export(curl, ssl_export_callback, (void *)0x9ABC);

    /* Fuzz curl_easy_getinfo with various info types */
    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    double total_time;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    char *content_type;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);

    curl_off_t size_download;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);

    /* Cleanup */
    curl_easy_cleanup(curl);
    free(url_buf);

    return 0;
}