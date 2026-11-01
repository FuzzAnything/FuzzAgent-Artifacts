// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
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
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback to satisfy type requirements if needed */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Set a basic URL to ensure a valid transfer context */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    /* Use fuzz data to drive option selection and values */
    unsigned int option = Data[0] % 300; /* CURLOPT range is roughly 0-300 */
    
    /* Attempt to set an option based on fuzz data */
    switch (option) {
        case CURLOPT_URL:
            if (Size > 1) {
                char *url_buf = malloc(Size);
                if (url_buf) {
                    memcpy(url_buf, Data + 1, Size - 1);
                    url_buf[Size - 1] = '\0';
                    curl_easy_setopt(curl, CURLOPT_URL, url_buf);
                    free(url_buf);
                }
            }
            break;
        case CURLOPT_POSTFIELDS:
            if (Size > 1) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)(Data + 1));
            }
            break;
        case CURLOPT_TIMEOUT:
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(Data[1] % 100));
            break;
        case CURLOPT_CONNECTTIMEOUT:
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long)(Data[1] % 100));
            break;
        case CURLOPT_VERBOSE:
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[1] % 2));
            break;
        case CURLOPT_FOLLOWLOCATION:
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(Data[1] % 2));
            break;
        case CURLOPT_USERAGENT:
            if (Size > 1) {
                char *ua_buf = malloc(Size);
                if (ua_buf) {
                    memcpy(ua_buf, Data + 1, Size - 1);
                    ua_buf[Size - 1] = '\0';
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, ua_buf);
                    free(ua_buf);
                }
            }
            break;
        case CURLOPT_INFILESIZE:
            curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)(Data[1]));
            break;
        case CURLOPT_NOPROXY:
            if (Size > 1) {
                char *proxy_buf = malloc(Size);
                if (proxy_buf) {
                    memcpy(proxy_buf, Data + 1, Size - 1);
                    proxy_buf[Size - 1] = '\0';
                    curl_easy_setopt(curl, CURLOPT_NOPROXY, proxy_buf);
                    free(proxy_buf);
                }
            }
            break;
        case CURLOPT_HTTPHEADER:
            if (Size > 1) {
                struct curl_slist *list = NULL;
                char *header_buf = malloc(Size);
                if (header_buf) {
                    memcpy(header_buf, Data + 1, Size - 1);
                    header_buf[Size - 1] = '\0';
                    list = curl_slist_append(list, header_buf);
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                    curl_slist_free_all(list);
                    free(header_buf);
                }
            }
            break;
        default:
            /* Try setting as long if it's a numeric option */
            if (option > 0 && option < CURLOPTTYPE_OBJECTPOINT) {
                curl_easy_setopt(curl, (CURLoption)option, (long)(Data[1]));
            } else if (option >= CURLOPTTYPE_OBJECTPOINT && option < CURLOPTTYPE_BLOB) {
                /* Try as string if possible */
                if (Size > 1) {
                    char *str_buf = malloc(Size);
                    if (str_buf) {
                        memcpy(str_buf, Data + 1, Size - 1);
                        str_buf[Size - 1] = '\0';
                        curl_easy_setopt(curl, (CURLoption)option, str_buf);
                        free(str_buf);
                    }
                }
            }
            break;
    }

    /* Perform the transfer */
    curl_easy_perform(curl);

    /* Call curl_easy_upkeep to test connection pool maintenance */
    curl_easy_upkeep(curl);

    /* Test curl_easy_getinfo with various info types */
    long http_code = 0;
    double total_time = 0.0;
    curl_off_t size_download = 0;
    char *content_type = NULL;
    struct curl_slist *cookies = NULL;
    curl_socket_t sock = -1;

    /* Test CURLINFO_RESPONSE_CODE */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    /* Test CURLINFO_TOTAL_TIME */
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    /* Test CURLINFO_SIZE_DOWNLOAD */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);

    /* Test CURLINFO_CONTENT_TYPE */
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);

    /* Test CURLINFO_COOKIELIST */
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (cookies) {
        curl_slist_free_all(cookies);
    }

    /* Test CURLINFO_ACTIVESOCKET */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}