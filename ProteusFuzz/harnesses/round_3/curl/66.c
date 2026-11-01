// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
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

/*
 * Fuzz driver for libcurl API functions.
 * Targets:
 * 1. curl_share_strerror (functional)
 * 2. curl_version (functional)
 * 3. curl_easy_setopt/getinfo with type-checking macros (Wcurl_*)
 */

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *dummy = "dummy content for curl testing\n";
        fwrite(dummy, 1, strlen(dummy), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Setup dummy file if needed */
    setup_dummy_file();

    /* Test curl_version() - no arguments, always safe */
    char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    /* Test curl_share_strerror() with various error codes */
    CURLSHcode error_code;
    if (Size >= sizeof(CURLSHcode)) {
        memcpy(&error_code, Data, sizeof(CURLSHcode));
        const char *str = curl_share_strerror(error_code);
        if (str) {
            (void)strlen(str);
        }
    } else {
        /* Fallback to known error codes */
        const char *str1 = curl_share_strerror(CURLSHE_OK);
        const char *str2 = curl_share_strerror(CURLSHE_IN_USE);
        const char *str3 = curl_share_strerror(CURLSHE_NOT_BUILT_IN);
        /* Use CURLSHE_LAST or a valid known code instead of undefined ones */
        const char *str4 = curl_share_strerror(CURLSHE_OK); 
        if (str1) (void)strlen(str1);
        if (str2) (void)strlen(str2);
        if (str3) (void)strlen(str3);
        if (str4) (void)strlen(str4);
    }

    /* Create a CURL handle for testing curl_easy_setopt/getinfo */
    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a share handle for testing */
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
        curl_easy_setopt(handle, CURLOPT_SHARE, share);
    }

    /* Use fuzzing data to drive curl_easy_setopt options */
    size_t idx = 0;
    while (idx < Size) {
        unsigned char option_byte = Data[idx++];
        curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");

        switch (option_byte % 10) {
            case 0:
                /* CURLOPT_FOLLOWLOCATION (long) */
                curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, option_byte);
                break;
            case 1:
                /* CURLOPT_TIMEOUT (long) */
                curl_easy_setopt(handle, CURLOPT_TIMEOUT, option_byte);
                break;
            case 2:
                /* CURLOPT_POSTFIELDS (void* or char*) */
                if (idx < Size) {
                    size_t len = Data[idx++] & 0x3F; /* limit length */
                    if (idx + len <= Size) {
                        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, (void *)&Data[idx]);
                        idx += len;
                    }
                }
                break;
            case 3:
                /* CURLOPT_USERAGENT (string) */
                if (idx < Size) {
                    size_t len = Data[idx++] & 0x3F;
                    if (idx + len <= Size) {
                        /* Ensure null termination */
                        char *ua = malloc(len + 1);
                        if (ua) {
                            memcpy(ua, &Data[idx], len);
                            ua[len] = '\0';
                            curl_easy_setopt(handle, CURLOPT_USERAGENT, ua);
                            free(ua);
                            idx += len;
                        }
                    }
                }
                break;
            case 4:
                /* CURLOPT_CAINFO (string) */
                curl_easy_setopt(handle, CURLOPT_CAINFO, "./dummy_file");
                break;
            case 5:
                /* CURLOPT_COOKIEFILE (string) */
                curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "./dummy_file");
                break;
            case 6:
                /* CURLOPT_HTTPHEADER (curl_slist*) */
                {
                    struct curl_slist *headers = NULL;
                    if (idx < Size) {
                        size_t len = Data[idx++] & 0x3F;
                        if (idx + len <= Size) {
                            char *header = malloc(len + 1);
                            if (header) {
                                memcpy(header, &Data[idx], len);
                                header[len] = '\0';
                                headers = curl_slist_append(headers, header);
                                free(header);
                                idx += len;
                            }
                        }
                    }
                    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
                    curl_slist_free_all(headers);
                }
                break;
            case 7:
                /* CURLOPT_ERRORBUFFER (char buffer) */
                {
                    char error_buffer[CURL_ERROR_SIZE];
                    memset(error_buffer, 0, CURL_ERROR_SIZE);
                    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
                }
                break;
            case 8:
                /* CURLOPT_WRITEDATA (void*) */
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&Data[idx]);
                break;
            case 9:
                /* CURLOPT_WRITEFUNCTION (callback) - use NULL to test */
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, NULL);
                break;
        }

        /* Test curl_easy_getinfo with various info types */
        if (idx < Size) {
            unsigned char info_byte = Data[idx++];
            long long_val = 0;
            double double_val = 0.0;
            char *string_val = NULL;
            struct curl_slist *slist_val = NULL;
            struct curl_certinfo *certinfo_val = NULL;
            struct curl_tlssessioninfo *tlsinfo_val = NULL;
            curl_socket_t socket_val = CURL_SOCKET_BAD;
            curl_off_t off_t_val = 0;

            switch (info_byte % 8) {
                case 0:
                    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &long_val);
                    break;
                case 1:
                    curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &double_val);
                    break;
                case 2:
                    curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &string_val);
                    if (string_val) (void)strlen(string_val);
                    break;
                case 3:
                    curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &slist_val);
                    if (slist_val) curl_slist_free_all(slist_val);
                    break;
                case 4:
                    curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo_val);
                    /* curl_certinfo_free is not a standard exported function in all versions,
                       and often the memory is managed internally or not freed by user.
                       We skip freeing to avoid compilation errors and potential crashes. */
                    if (certinfo_val) {
                        /* Just access to ensure it's valid, no free call */
                        (void)certinfo_val;
                    }
                    break;
                case 5:
                    curl_easy_getinfo(handle, CURLINFO_TLS_SESSION, &tlsinfo_val);
                    break;
                case 6:
                    curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &socket_val);
                    break;
                case 7:
                    curl_easy_getinfo(handle, CURLINFO_SIZE_UPLOAD_T, &off_t_val);
                    break;
            }
        }
    }

    /* Perform a dummy request to exercise more states */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_perform(handle);

    /* Cleanup: Ensure handle is cleaned up before share to avoid dangling references */
    curl_easy_cleanup(handle);
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    curl_global_cleanup();

    return 0;
}