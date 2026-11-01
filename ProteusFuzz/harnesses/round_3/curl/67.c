// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback to satisfy CURLOPT_HSTSWRITEFUNCTION type checking */
static size_t dummy_hstswrite_cb(void *userptr, char *data, size_t size) {
    (void)userptr;
    (void)data;
    (void)size;
    return size;
}

/* Dummy write callback */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Dummy read callback */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Allocate error buffer dynamically to avoid stack issues */
    char *error_buffer = malloc(CURL_ERROR_SIZE);
    if (!error_buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memset(error_buffer, 0, CURL_ERROR_SIZE);

    /* Set basic options to ensure perform has something to do */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);

    /* Use fuzzer data to drive options and getinfo calls */
    size_t idx = 0;
    while (idx < Size) {
        uint8_t op = Data[idx];
        idx++;

        /* Use modulo to pick a safe range of CURLoption and CURLINFO */
        /* We limit options to a safe subset to avoid crashes from invalid types */
        if (op % 2 == 0) {
            /* Try setting an option */
            CURLoption opt = (CURLoption)(op % 100);
            /* Only set options that take simple types or NULL to avoid complex callbacks */
            if (opt == CURLOPT_TIMEOUT) {
                long val = (idx < Size) ? Data[idx++] : 0;
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, val);
            } else if (opt == CURLOPT_VERBOSE) {
                long val = (idx < Size) ? Data[idx++] : 0;
                curl_easy_setopt(curl, CURLOPT_VERBOSE, val);
            } else if (opt == CURLOPT_FOLLOWLOCATION) {
                long val = (idx < Size) ? Data[idx++] : 0;
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, val);
            } else if (opt == CURLOPT_HSTS) {
                /* Test string option with fuzzer data */
                size_t len = (idx < Size) ? Data[idx++] : 0;
                if (idx + len <= Size) {
                    curl_easy_setopt(curl, CURLOPT_HSTS, (char *)&Data[idx]);
                    idx += len;
                }
            } else if (opt == CURLOPT_HSTSWRITEFUNCTION) {
                /* Test the specific callback type check */
                curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
            }
        } else {
            /* Try getting info */
            CURLINFO info = (CURLINFO)(op % 100);
            if (info == CURLINFO_HTTP_CODE) {
                long code;
                curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &code);
            } else if (info == CURLINFO_SIZE_DOWNLOAD) {
                double size;
                curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size);
            } else if (info == CURLINFO_CONTENT_TYPE) {
                char *content_type;
                curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
            } else if (info == CURLINFO_PRIVATE) {
                char *priv;
                curl_easy_getinfo(curl, CURLINFO_PRIVATE, &priv);
            } else if (info == CURLINFO_COOKIELIST) {
                struct curl_slist *list;
                curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &list);
                if (list) {
                    curl_slist_free_all(list);
                }
            }
        }
    }

    /* Perform the transfer */
    curl_easy_perform(curl);

    /* Perform upkeep */
    curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    free(error_buffer);

    return 0;
}