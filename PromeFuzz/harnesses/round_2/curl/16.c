// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"

static CURLcode dummy_export_cb(void *userptr, void *reserved,
                               const char *session_key,
                               const unsigned char *shmac, size_t shmac_len,
                               const unsigned char *sdata, size_t sdata_len,
                               long validfor, int flags,
                               const char *cipher, size_t cipher_len) {
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    /* Initialize global curl state if needed */
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }

    /* Create share and easy handles */
    CURLSH *share = curl_share_init();
    CURL *easy = curl_easy_init();

    if (share && easy) {
        /* Test curl_share_setopt with safe options */
        if (Size > 0) {
            /* Use only valid CURLSHoption values */
            CURLSHoption options[] = {
                CURLSHOPT_SHARE,
                CURLSHOPT_UNSHARE,
                CURLSHOPT_USERDATA
            };
            int num_options = sizeof(options) / sizeof(options[0]);
            CURLSHoption option = options[Data[0] % num_options];
            
            /* Use safe values for each option */
            long value = 0;
            if (Size > 1) {
                value = (long)Data[1];
            }
            
            /* For SHARE/UNSHARE, use only valid CURL_LOCK_DATA values */
            if (option == CURLSHOPT_SHARE || option == CURLSHOPT_UNSHARE) {
                value = value & (CURL_LOCK_DATA_COOKIE | 
                               CURL_LOCK_DATA_DNS | 
                               CURL_LOCK_DATA_SSL_SESSION);
            }
            
            CURLSHcode setopt_result = curl_share_setopt(share, option, value);
            
            /* Test curl_share_strerror with the result */
            const char *error_str = curl_share_strerror(setopt_result);
            (void)error_str;
        }

        /* Test SSL session import with bounds checking */
        if (Size >= 10) {
            /* Create session key from fuzzer data with null termination */
            char session_key[64] = {0};
            size_t key_len = Size < 63 ? Size : 63;
            if (key_len > 0) {
                memcpy(session_key, Data, key_len);
                session_key[key_len] = '\0';
            }
            
            /* Use remaining data for shmac and sdata with proper bounds */
            size_t remaining = Size - key_len;
            const unsigned char *shmac = NULL;
            size_t shmac_len = 0;
            const unsigned char *sdata = NULL;
            size_t sdata_len = 0;
            
            if (remaining > 0) {
                shmac = Data + key_len;
                shmac_len = remaining > 32 ? 32 : remaining;
                
                if (remaining > 32) {
                    sdata = Data + key_len + 32;
                    sdata_len = remaining - 32;
                    if (sdata_len > 1024) {
                        sdata_len = 1024;
                    }
                }
            }

            /* Test curl_easy_ssls_import */
            CURLcode import_result = curl_easy_ssls_import(easy, session_key,
                                                          shmac, shmac_len,
                                                          sdata, sdata_len);
            (void)import_result;
            
            /* Test curl_easy_ssls_export */
            CURLcode export_result = curl_easy_ssls_export(easy, dummy_export_cb, NULL);
            (void)export_result;
        }
    }

    /* Clean up share handle */
    if (share) {
        CURLSHcode cleanup_result = curl_share_cleanup(share);
        (void)cleanup_result;
    }

    /* Clean up easy handle */
    if (easy) {
        curl_easy_cleanup(easy);
    }

    return 0;
}