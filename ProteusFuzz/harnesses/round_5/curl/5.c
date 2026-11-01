// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/* Correct signature for curl_ssls_export_cb based on the error message */
static CURLcode ssls_export_callback(void *userptr,
                                     void *sslptr,
                                     const char *ssltype,
                                     const unsigned char *cert,
                                     unsigned long certlen,
                                     const unsigned char *key,
                                     unsigned long keylen,
                                     long version,
                                     int is_server,
                                     const char *cipher,
                                     unsigned long cipherlen) {
    (void)userptr;
    (void)sslptr;
    (void)ssltype;
    (void)cert;
    (void)certlen;
    (void)key;
    (void)keylen;
    (void)version;
    (void)is_server;
    (void)cipher;
    (void)cipherlen;
    return CURLE_OK;
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Create a dummy file for potential file-based operations */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Prepare a null-terminated string from input for options */
    char *safe_string = NULL;
    if (Size > 0) {
        safe_string = (char *)malloc(Size + 1);
        if (safe_string) {
            memcpy(safe_string, Data, Size);
            safe_string[Size] = '\0';
        }
    }

    /* Setup basic options to ensure handle is valid for perform */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);

    if (safe_string) {
        /* Fuzz various string options */
        curl_easy_setopt(handle, CURLOPT_USERAGENT, safe_string);
        curl_easy_setopt(handle, CURLOPT_PROXY, safe_string);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, safe_string);
        curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, safe_string);
    }

    /* Test curl_easy_perform */
    curl_easy_perform(handle);

    /* Test curl_easy_pause with different bitmasks */
    curl_easy_pause(handle, CURLPAUSE_RECV);
    curl_easy_pause(handle, CURLPAUSE_SEND);
    curl_easy_pause(handle, CURLPAUSE_ALL);
    curl_easy_pause(handle, CURLPAUSE_CONT);

    /* Test curl_easy_upkeep */
    curl_easy_upkeep(handle);

    /* Test curl_easy_ssls_export if compiled in */
    curl_easy_ssls_export(handle, ssls_export_callback, NULL);

    /* Test MIME related functions */
    curl_mime *mime = curl_mime_init(handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Set some data for the part */
            if (safe_string) {
                curl_mime_data(part, safe_string, Size);
            } else {
                curl_mime_data(part, "dummy", 5);
            }

            /* Create a header list */
            struct curl_slist *headers = NULL;
            if (safe_string) {
                headers = curl_slist_append(headers, safe_string);
            } else {
                headers = curl_slist_append(headers, "Content-Type: text/plain");
            }

            if (headers) {
                /* Test curl_mime_headers */
                curl_mime_headers(part, headers, 1);
            }
        }
        curl_mime_free(mime);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    free(safe_string);

    return 0;
}