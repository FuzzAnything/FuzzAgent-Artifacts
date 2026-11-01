// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback for curl_easy_ssls_export matching the expected signature */
static CURLcode dummy_ssls_export_cb(void *userptr,
                                     void *session,
                                     const char *id,
                                     const unsigned char *id_data,
                                     unsigned long id_len,
                                     const unsigned char *cipher,
                                     unsigned long cipher_len,
                                     long version,
                                     int type,
                                     const char *protocol,
                                     unsigned long protocol_len) {
    (void)userptr;
    (void)session;
    (void)id;
    (void)id_data;
    (void)id_len;
    (void)cipher;
    (void)cipher_len;
    (void)version;
    (void)type;
    (void)protocol;
    (void)protocol_len;
    return CURLE_OK;
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

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize variables for getinfo */
    long long_val = 0;
    double double_val = 0.0;
    curl_off_t off_t_val = 0;
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    char *string_val = NULL;
    struct curl_slist *slist_val = NULL;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;

    /* Use input data to drive fuzzing decisions */
    unsigned int option_choice = Data[0] % 100;
    unsigned int info_choice = (Size > 1) ? Data[1] % 50 : 0;

    /* Set a basic URL to ensure perform() has something to do */
    const char *url = "http://example.com/fuzz";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);

    /* Fuzz various setopt options based on input */
    if (option_choice < 10) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    } else if (option_choice < 20) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    } else if (option_choice < 30) {
        /* Set a custom header */
        /* CRITICAL FIX: Do not free headers immediately. 
           curl_easy_setopt takes ownership of the list. 
           curl_easy_cleanup will free it later. */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Fuzzer");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        /* Removed: curl_slist_free_all(headers); */
    } else if (option_choice < 40) {
        /* Set POST fields */
        if (Size > 5) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (const char *)Data + 5);
        }
    } else if (option_choice < 50) {
        /* Set callback data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
    }

    /* Perform the transfer */
    curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo with various CURLINFO options */
    /* Map info_choice to specific CURLINFO enums */
    CURLINFO info_enum;
    switch (info_choice) {
        case 0: info_enum = CURLINFO_RESPONSE_CODE; break;
        case 1: info_enum = CURLINFO_HTTP_CODE; break;
        case 2: info_enum = CURLINFO_SIZE_DOWNLOAD; break;
        case 3: info_enum = CURLINFO_SIZE_UPLOAD; break;
        case 4: info_enum = CURLINFO_SPEED_DOWNLOAD; break;
        case 5: info_enum = CURLINFO_SPEED_UPLOAD; break;
        case 6: info_enum = CURLINFO_CONNECT_TIME; break;
        case 7: info_enum = CURLINFO_STARTTRANSFER_TIME; break;
        case 8: info_enum = CURLINFO_TOTAL_TIME; break;
        case 9: info_enum = CURLINFO_NAMELOOKUP_TIME; break;
        case 10: info_enum = CURLINFO_PRETRANSFER_TIME; break;
        case 11: info_enum = CURLINFO_REDIRECT_TIME; break;
        case 12: info_enum = CURLINFO_REDIRECT_COUNT; break;
        case 13: info_enum = CURLINFO_EFFECTIVE_URL; break;
        case 14: info_enum = CURLINFO_SSL_ENGINES; break;
        case 15: info_enum = CURLINFO_COOKIELIST; break;
        case 16: info_enum = CURLINFO_LASTSOCKET; break;
        case 17: info_enum = CURLINFO_OS_ERRNO; break;
        case 18: info_enum = CURLINFO_CONTENT_LENGTH_DOWNLOAD_T; break;
        case 19: info_enum = CURLINFO_CONTENT_LENGTH_UPLOAD_T; break;
        case 20: info_enum = CURLINFO_SIZE_DOWNLOAD_T; break;
        case 21: info_enum = CURLINFO_SIZE_UPLOAD_T; break;
        case 22: info_enum = CURLINFO_SPEED_DOWNLOAD_T; break;
        case 23: info_enum = CURLINFO_SPEED_UPLOAD_T; break;
        case 24: info_enum = CURLINFO_CONTENT_TYPE; break;
        case 25: info_enum = CURLINFO_PRIVATE; break;
        case 26: info_enum = CURLINFO_TLS_SESSION; break;
        case 27: info_enum = CURLINFO_TLS_SSL_PTR; break;
        case 28: info_enum = CURLINFO_CERTINFO; break;
        case 29: info_enum = CURLINFO_APPCONNECT_TIME; break;
        case 30: info_enum = CURLINFO_RTSP_CLIENT_CSEQ; break;
        case 31: info_enum = CURLINFO_RTSP_CSEQ_RECV; break;
        case 32: info_enum = CURLINFO_RTSP_SERVER_CSEQ; break;
        case 33: info_enum = CURLINFO_RTSP_SESSION_ID; break;
        case 34: info_enum = CURLINFO_PRIMARY_IP; break;
        case 35: info_enum = CURLINFO_PRIMARY_PORT; break;
        case 36: info_enum = CURLINFO_LOCAL_IP; break;
        case 37: info_enum = CURLINFO_LOCAL_PORT; break;
        case 38: info_enum = CURLINFO_NUM_CONNECTS; break;
        case 39: info_enum = CURLINFO_FTP_ENTRY_PATH; break;
        case 40: info_enum = CURLINFO_REDIRECT_URL; break;
        case 41: info_enum = CURLINFO_ACTIVESOCKET; break;
        case 42: info_enum = CURLINFO_HTTP_VERSION; break;
        case 43: info_enum = CURLINFO_PROXY_SSL_VERIFYRESULT; break;
        case 44: info_enum = CURLINFO_PROTOCOL; break;
        case 45: info_enum = CURLINFO_SCHEME; break;
        case 46: info_enum = CURLINFO_CONDITION_UNMET; break;
        case 47: info_enum = CURLINFO_HTTPAUTH_AVAIL; break;
        case 48: info_enum = CURLINFO_PROXYAUTH_AVAIL; break;
        case 49: info_enum = CURLINFO_OS_ERRNO; break;
        default: info_enum = CURLINFO_RESPONSE_CODE; break;
    }

    /* Call curl_easy_getinfo based on the type expected by the info enum */
    if (info_choice < 12) {
        /* Long types */
        curl_easy_getinfo(curl, info_enum, &long_val);
    } else if (info_choice < 16) {
        /* Double types */
        curl_easy_getinfo(curl, info_enum, &double_val);
    } else if (info_choice < 19) {
        /* Off_t types */
        curl_easy_getinfo(curl, info_enum, &off_t_val);
    } else if (info_choice == 19) {
        /* Socket types */
        curl_easy_getinfo(curl, info_enum, &socket_val);
    } else if (info_choice == 20 || info_choice == 21 || info_choice == 22 || info_choice == 23 || info_choice == 24 || info_choice == 25) {
        /* String types */
        curl_easy_getinfo(curl, info_enum, &string_val);
    } else if (info_choice == 26 || info_choice == 27) {
        /* Slist types - SSL_ENGINES and COOKIELIST */
        curl_easy_getinfo(curl, info_enum, &slist_val);
    } else if (info_choice == 28 || info_choice == 29) {
        /* TLS Session types */
        curl_easy_getinfo(curl, info_enum, &tls_info);
    } else if (info_choice == 30) {
        /* Certinfo types */
        curl_easy_getinfo(curl, info_enum, &cert_info);
    } else {
        /* Fallback to long for unknown mappings */
        curl_easy_getinfo(curl, info_enum, &long_val);
    }

    /* Cleanup slist if allocated by getinfo (Critical fix for LeakSanitizer) */
    if (slist_val) {
        curl_slist_free_all(slist_val);
    }

    /* Test curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Test curl_easy_ssls_export */
    curl_easy_ssls_export(curl, dummy_ssls_export_cb, (void *)Data);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}