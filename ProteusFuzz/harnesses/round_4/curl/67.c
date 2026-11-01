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
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback functions to satisfy type requirements */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    (void)handle; (void)type; (void)data; (void)size; (void)userp;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

/* Helper to safely get an integer from fuzz data */
static int get_int(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int) > Size) {
        *offset = Size;
        return 0;
    }
    int val;
    memcpy(&val, Data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/* Helper to safely get a long from fuzz data */
static long get_long(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(long) > Size) {
        *offset = Size;
        return 0;
    }
    long val;
    memcpy(&val, Data + *offset, sizeof(long));
    *offset += sizeof(long);
    return val;
}

/* Helper to safely get a double from fuzz data */
static double get_double(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(double) > Size) {
        *offset = Size;
        return 0.0;
    }
    double val;
    memcpy(&val, Data + *offset, sizeof(double));
    *offset += sizeof(double);
    return val;
}

/* Helper to safely get a curl_off_t from fuzz data */
static curl_off_t get_off_t(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(curl_off_t) > Size) {
        *offset = Size;
        return 0;
    }
    curl_off_t val;
    memcpy(&val, Data + *offset, sizeof(curl_off_t));
    *offset += sizeof(curl_off_t);
    return val;
}

/* Helper to safely get a curl_socket_t from fuzz data */
static curl_socket_t get_socket_t(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(curl_socket_t) > Size) {
        *offset = Size;
        return CURL_SOCKET_BAD;
    }
    curl_socket_t val;
    memcpy(&val, Data + *offset, sizeof(curl_socket_t));
    *offset += sizeof(curl_socket_t);
    return val;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    size_t offset = 0;
    int op_code = get_int(Data, Size, &offset);
    int action = op_code % 10;

    /* Prepare dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }

    /* Set some basic options to make the handle usable */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

    /* Execute based on action type */
    switch (action) {
        case 0: {
            /* Test curl_easy_setopt with various option types */
            int opt_val = get_int(Data, Size, &offset);
            long long_val = get_long(Data, Size, &offset);
            double double_val = get_double(Data, Size, &offset);
            curl_off_t off_val = get_off_t(Data, Size, &offset);
            
            /* Try setting different types of options */
            curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_val % 2);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val % 100 + 1);
            curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, (long)(double_val * 10));
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)(off_val % 1000));
            
            /* Try string options if data allows */
            if (offset + 10 <= Size) {
                char str_buf[11] = {0};
                memcpy(str_buf, Data + offset, 10);
                offset += 10;
                curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, str_buf);
            }
            break;
        }
        case 1: {
            /* Test curl_easy_perform */
            curl_easy_perform(curl);
            break;
        }
        case 2: {
            /* Test curl_easy_getinfo with various info types */
            long long_info;
            double double_info;
            curl_off_t off_info;
            curl_socket_t socket_info;
            char *string_info;
            struct curl_slist *slist_info;
            struct curl_tlssessioninfo *tls_info;
            struct curl_certinfo *cert_info;

            int info_type = get_int(Data, Size, &offset);
            
            switch (info_type % 10) {
                case 0:
                    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &long_info);
                    break;
                case 1:
                    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_info);
                    break;
                case 2:
                    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &off_info);
                    break;
                case 3:
                    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
                    break;
                case 4:
                    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_info);
                    break;
                case 5:
                    curl_easy_getinfo(curl, CURLINFO_SSL_ENGINES, &slist_info);
                    if (slist_info) curl_slist_free_all(slist_info);
                    break;
                case 6:
                    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
                    break;
                case 7:
                    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
                    break;
                case 8:
                    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
                    if (slist_info) curl_slist_free_all(slist_info);
                    break;
                case 9:
                    curl_easy_getinfo(curl, CURLINFO_PRIVATE, &string_info);
                    break;
            }
            break;
        }
        case 3: {
            /* Test curl_easy_pause */
            int bitmask = get_int(Data, Size, &offset);
            /* Use valid pause masks */
            bitmask = bitmask & (CURLPAUSE_RECV | CURLPAUSE_SEND | CURLPAUSE_CONT);
            curl_easy_pause(curl, bitmask);
            break;
        }
        case 4: {
            /* Test curl_easy_upkeep */
            curl_easy_upkeep(curl);
            break;
        }
        case 5: {
            /* Combined: setopt, perform, getinfo */
            curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            curl_easy_perform(curl);
            long http_code;
            curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
            break;
        }
        case 6: {
            /* Test with file options */
            if (dummy_file) {
                curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
                curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./dummy_file");
                curl_easy_setopt(curl, CURLOPT_CAINFO, "./dummy_file");
            }
            curl_easy_perform(curl);
            break;
        }
        case 7: {
            /* Test with list options */
            struct curl_slist *list = NULL;
            if (offset + 20 <= Size) {
                char header1[21] = {0};
                char header2[21] = {0};
                memcpy(header1, Data + offset, 20);
                offset += 20;
                if (offset + 20 <= Size) {
                    memcpy(header2, Data + offset, 20);
                    offset += 20;
                    list = curl_slist_append(list, header1);
                    list = curl_slist_append(list, header2);
                } else {
                    list = curl_slist_append(list, header1);
                }
            }
            if (list) {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                curl_easy_perform(curl);
                curl_slist_free_all(list);
            }
            break;
        }
        case 8: {
            /* Test with callback data */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(size_t)0x12345678);
            curl_easy_setopt(curl, CURLOPT_READDATA, (void*)(size_t)0x87654321);
            curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)(size_t)0xDEADBEEF);
            curl_easy_perform(curl);
            break;
        }
        case 9: {
            /* Test error handling with invalid options */
            /* Use an invalid option value by casting an integer to CURLoption */
            /* This tests the error path without using an undefined macro */
            curl_easy_setopt(curl, (CURLoption)0x99999999, 12345);
            curl_easy_perform(curl);
            break;
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    return 0;
}