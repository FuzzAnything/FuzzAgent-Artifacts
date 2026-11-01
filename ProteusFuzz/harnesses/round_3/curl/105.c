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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy callback for testing */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

static int dummy_hstsread_callback(CURL *handle, const char *domain, char *buffer, size_t buflen, void *userdata) {
    (void)handle;
    (void)domain;
    (void)buffer;
    (void)buflen;
    (void)userdata;
    return 0;
}

static int dummy_hstswrite_callback(CURL *handle, const char *domain, const char *value, void *userdata) {
    (void)handle;
    (void)domain;
    (void)value;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize a dummy file for file-based options if needed */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Use input data to drive fuzzing logic */
    unsigned int option_type = Data[0] % 100;
    unsigned int info_type = Data[1] % 100;

    /* Fuzz curl_easy_setopt with various option types */
    if (option_type < 10) {
        /* String options */
        curl_easy_setopt(curl, CURLOPT_URL, (char *)Data);
    } else if (option_type < 20) {
        /* Long options */
        long val = (long)(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, val);
    } else if (option_type < 30) {
        /* Callback options */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
        curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
    } else if (option_type < 40) {
        /* slist options */
        struct curl_slist *slist = curl_slist_append(NULL, (char *)Data);
        if (slist) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
            curl_slist_free_all(slist);
        }
    } else if (option_type < 50) {
        /* FILE options */
        FILE *fp = fopen("./dummy_file", "r");
        if (fp) {
            curl_easy_setopt(curl, CURLOPT_READDATA, fp);
            fclose(fp);
        }
    } else if (option_type < 60) {
        /* POST fields */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
    } else if (option_type < 70) {
        /* SSL options */
        curl_easy_setopt(curl, CURLOPT_SSLCERT, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLKEY, "./dummy_file");
    } else if (option_type < 80) {
        /* Proxy options */
        curl_easy_setopt(curl, CURLOPT_PROXY, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, (char *)Data);
    } else if (option_type < 90) {
        /* Cookie options */
        curl_easy_setopt(curl, CURLOPT_COOKIE, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
    } else {
        /* Miscellaneous options */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_REFERER, (char *)Data);
    }

    /* Fuzz curl_easy_getinfo with various info types */
    if (info_type < 10) {
        /* String info */
        char *str_info = NULL;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_info);
    } else if (info_type < 20) {
        /* Long info */
        long long_info = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    } else if (info_type < 30) {
        /* Double info */
        double double_info = 0.0;
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
    } else if (info_type < 40) {
        /* slist info */
        struct curl_slist *slist_info = NULL;
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
        if (slist_info) {
            curl_slist_free_all(slist_info);
        }
    } else if (info_type < 50) {
        /* Certinfo */
        struct curl_certinfo *certinfo = NULL;
        curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
        if (certinfo) {
            /* Cleanup handled by curl_slist_free_all internally if needed */
        }
    } else if (info_type < 60) {
        /* Socket info */
        curl_socket_t socket_info = CURL_SOCKET_BAD;
        curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
    } else if (info_type < 70) {
        /* Off_t info */
        curl_off_t off_info = 0;
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_info);
    } else if (info_type < 80) {
        /* TLS session info */
        struct curl_tlssessioninfo *tls_info = NULL;
        curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
    } else if (info_type < 90) {
        /* Private data */
        char *private_info = NULL;
        curl_easy_getinfo(curl, CURLINFO_PRIVATE, &private_info);
    } else {
        /* SSL engines */
        struct curl_slist *ssl_engines = NULL;
        curl_easy_getinfo(curl, CURLINFO_SSL_ENGINES, &ssl_engines);
        if (ssl_engines) {
            curl_slist_free_all(ssl_engines);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    return 0;
}