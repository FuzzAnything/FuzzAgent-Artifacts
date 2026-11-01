// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callbacks to satisfy type requirements */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr; (void)size; (void)nmemb; (void)userptr;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr; (void)size; (void)nmemb; (void)userptr;
    return size * nmemb;
}

static int dummy_hstsread_cb(void *userptr, const char *url, char *buf, size_t buflen) {
    (void)userptr; (void)url; (void)buf; (void)buflen;
    return 0;
}

static int dummy_hstswrite_cb(void *userptr, const char *url, const char *data, size_t len) {
    (void)userptr; (void)url; (void)data; (void)len;
    return 0;
}

/* Correct signature for curl_ssls_export_cb based on curl.h */
static CURLcode dummy_ssls_export_cb(void *userptr, void *ssl_session, 
                                     const char *hostname, 
                                     const unsigned char *session_id, 
                                     unsigned long session_id_len, 
                                     const unsigned char *cipher, 
                                     unsigned long cipher_len, 
                                     long version, 
                                     int type, 
                                     const char *protocol, 
                                     unsigned long protocol_len) {
    (void)userptr; (void)ssl_session; (void)hostname; (void)session_id; 
    (void)session_id_len; (void)cipher; (void)cipher_len; (void)version; 
    (void)type; (void)protocol; (void)protocol_len;
    return CURLE_OK;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

/* Helper to safely get a byte from input */
static uint8_t get_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Use input data to drive fuzzing logic */
    uint8_t op = get_byte(Data, Size, 0) % 10;
    char buffer[256];
    size_t len = Size < sizeof(buffer) ? Size : sizeof(buffer);
    memcpy(buffer, Data, len);
    buffer[len] = '\0';

    /* Set some basic options to ensure handle is configured */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* Fuzz different API functions based on input */
    switch (op) {
        case 0:
            /* Test curl_easy_setopt with various options */
            if (len > 0) {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, buffer);
            }
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, buffer);
            break;

        case 1:
            /* Test curl_easy_setopt with callback options */
            curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
            curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void*)(size_t)get_byte(Data, Size, 1));
            curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
            curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, (void*)(size_t)get_byte(Data, Size, 1));
            break;

        case 2:
            /* Test curl_easy_perform */
            (void)curl_easy_perform(curl);
            break;

        case 3:
            /* Test curl_easy_getinfo */
            long response_code = 0;
            double total_time = 0.0;
            char *url = NULL;
            (void)curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            (void)curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            (void)curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
            break;

        case 4:
            /* Test curl_easy_upkeep */
            (void)curl_easy_upkeep(curl);
            break;

        case 5:
            /* Test curl_easy_ssls_export */
            (void)curl_easy_ssls_export(curl, dummy_ssls_export_cb, (void*)(size_t)get_byte(Data, Size, 1));
            break;

        case 6:
            /* Test with slist options */
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "User-Agent: Fuzz");
            headers = curl_slist_append(headers, "Accept: */*");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_slist_free_all(headers);
            break;

        case 7:
            /* Test with file options */
            FILE *fp = fopen("./dummy_file", "w");
            if (fp) {
                fwrite(buffer, 1, len, fp);
                fclose(fp);
                curl_easy_setopt(curl, CURLOPT_READDATA, NULL);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
            }
            break;

        case 8:
            /* Test with long options */
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)get_byte(Data, Size, 1));
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long)get_byte(Data, Size, 2));
            break;

        case 9:
            /* Test with off_t options */
            curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, (curl_off_t)get_byte(Data, Size, 1));
            break;
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    return 0;
}