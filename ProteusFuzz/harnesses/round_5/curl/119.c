// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <curl/typecheck-gcc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy callback to satisfy curl_easy_setopt requirements */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)userp;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize a dummy file for FILE* options */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }
    dummy_file = fopen("./dummy_file", "r");

    /* Prepare data buffers */
    char *url_str = NULL;
    char *header_str = NULL;
    long long_val = 0;
    double double_val = 0.0;
    curl_off_t off_val = 0;
    curl_socket_t sock_val = CURL_SOCKET_BAD;
    char *string_out = NULL;
    long long_out = 0;
    double double_out = 0.0;
    curl_off_t off_out = 0;
    curl_socket_t sock_out = CURL_SOCKET_BAD;
    struct curl_slist *slist_out = NULL;
    struct curl_tlssessioninfo tls_info;
    struct curl_certinfo cert_info;

    /* Parse input to determine test scenario */
    unsigned int op = 0;
    if (Size > 0) {
        op = Data[0] % 100;
    }

    /* Initialize common options */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);

    /* Test various setopt/getinfo scenarios based on input */
    switch (op) {
        case 0:
            /* Test FILE* option (CURLOPT_INFILE) */
            if (dummy_file) {
                curl_easy_setopt(curl, CURLOPT_INFILE, dummy_file);
            }
            break;
        case 1:
            /* Test string option (CURLOPT_URL) */
            if (Size > 1) {
                url_str = (char *)malloc(Size);
                if (url_str) {
                    memcpy(url_str, Data + 1, Size - 1);
                    url_str[Size - 1] = '\0';
                    curl_easy_setopt(curl, CURLOPT_URL, url_str);
                }
            }
            break;
        case 2:
            /* Test long option (CURLOPT_TIMEOUT) */
            if (Size > 1) {
                long_val = *(long *)(Data + 1);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
            }
            break;
        case 3:
            /* Test slist option (CURLOPT_HTTPHEADER) */
            if (Size > 1) {
                header_str = (char *)malloc(Size);
                if (header_str) {
                    memcpy(header_str, Data + 1, Size - 1);
                    header_str[Size - 1] = '\0';
                    struct curl_slist *list = curl_slist_append(NULL, header_str);
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                    curl_slist_free_all(list);
                }
            }
            break;
        case 4:
            /* Test POSTFIELDS option */
            if (Size > 1) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)(Data + 1));
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size - 1);
            }
            break;
        case 5:
            /* Test CURLOPT_POSTFIELDSIZE_LARGE (off_t) */
            if (Size > 1) {
                off_val = *(curl_off_t *)(Data + 1);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, off_val);
            }
            break;
        case 6:
            /* Test callback data option */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
            break;
        case 7:
            /* Test getinfo string */
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_out);
            break;
        case 8:
            /* Test getinfo long */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_out);
            break;
        case 9:
            /* Test getinfo double */
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &double_out);
            break;
        case 10:
            /* Test getinfo off_t */
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &off_out);
            break;
        case 11:
            /* Test getinfo socket */
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_out);
            break;
        case 12:
            /* Test getinfo slist */
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_out);
            if (slist_out) {
                curl_slist_free_all(slist_out);
            }
            break;
        case 13:
            /* Test getinfo tlssessioninfo */
            memset(&tls_info, 0, sizeof(tls_info));
            curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
            break;
        case 14:
            /* Test getinfo certinfo */
            memset(&cert_info, 0, sizeof(cert_info));
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
            break;
        default:
            /* Random option setting to explore more states */
            if (Size > 1) {
                long random_opt = *(long *)(Data + 1);
                curl_easy_setopt(curl, (CURLoption)random_opt, (void *)Data);
            }
            break;
    }

    /* Cleanup */
    if (dummy_file) {
        fclose(dummy_file);
    }
    if (url_str) {
        free(url_str);
    }
    if (header_str) {
        free(header_str);
    }
    curl_easy_cleanup(curl);

    return 0;
}