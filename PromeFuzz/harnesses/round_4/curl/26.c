// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"

/* Dummy callback for curl_easy_ssls_export with correct signature */
static CURLcode dummy_export_cb(void *userptr,
                               void *session_key,
                               const char *session_key_str,
                               const unsigned char *shmac,
                               unsigned long shmac_len,
                               const unsigned char *sdata,
                               unsigned long sdata_len,
                               long flags,
                               int age,
                               const char *cipher,
                               unsigned long cipher_len) {
    /* Do nothing, just return success */
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    /* Initialize handles */
    CURL *easy_handle = curl_easy_init();
    CURLSH *share_handle = curl_share_init();
    
    if (!easy_handle || !share_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (share_handle) curl_share_cleanup(share_handle);
        return 0;
    }
    
    /* Use the first byte to determine which function to test */
    uint8_t function_selector = 0;
    if (Size > 0) {
        function_selector = Data[0] % 6;
        Data++;
        Size--;
    }
    
    switch (function_selector) {
        case 0: { /* curl_easy_ssls_import */
            /* Prepare parameters from fuzz data */
            const char *session_key = (Size > 0) ? (const char *)Data : "";
            size_t key_len = (Size > 0) ? strnlen((const char *)Data, Size) : 0;
            
            /* Use remaining data for shmac and sdata */
            const unsigned char *shmac = NULL;
            size_t shmac_len = 0;
            const unsigned char *sdata = NULL;
            size_t sdata_len = 0;
            
            if (Size > key_len + 1) {
                shmac = Data + key_len + 1;
                shmac_len = (Size - key_len - 1) / 2;
                if (shmac_len > 0) {
                    sdata = shmac + shmac_len;
                    sdata_len = Size - key_len - 1 - shmac_len;
                }
            }
            
            CURLcode res = curl_easy_ssls_import(easy_handle, session_key,
                                                shmac, shmac_len,
                                                sdata, sdata_len);
            (void)res; /* Ignore result */
            break;
        }
        
        case 1: { /* curl_share_strerror */
            /* Generate error code from fuzz data */
            CURLSHcode error_code = (Size > 0) ? (CURLSHcode)(Data[0] % 10) : CURLSHE_OK;
            const char *error_str = curl_share_strerror(error_code);
            (void)error_str; /* Ignore result */
            break;
        }
        
        case 2: { /* curl_share_cleanup */
            /* Create a temporary share handle to clean up */
            CURLSH *temp_share = curl_share_init();
            if (temp_share) {
                CURLSHcode res = curl_share_cleanup(temp_share);
                (void)res; /* Ignore result */
            }
            break;
        }
        
        case 3: { /* curl_share_init */
            /* Already initialized at the beginning, but we can create another */
            CURLSH *temp_share = curl_share_init();
            if (temp_share) {
                curl_share_cleanup(temp_share);
            }
            break;
        }
        
        case 4: { /* curl_share_setopt */
            /* Try different options based on fuzz data */
            CURLSHoption option = CURLSHOPT_NONE;
            long value = 0;
            
            if (Size >= 2) {
                /* Use first byte for option, second for value */
                option = (CURLSHoption)(Data[0] % 5); /* 0-4 for common options */
                value = (long)(Data[1] % 3); /* 0-2 for common values */
            }
            
            /* Map to actual option values */
            switch (option) {
                case 0: option = CURLSHOPT_SHARE; break;
                case 1: option = CURLSHOPT_UNSHARE; break;
                case 2: option = CURLSHOPT_LOCKFUNC; break;
                case 3: option = CURLSHOPT_UNLOCKFUNC; break;
                case 4: option = CURLSHOPT_USERDATA; break;
                default: option = CURLSHOPT_SHARE; break;
            }
            
            /* Map to actual value types */
            long actual_value = 0;
            switch (value) {
                case 0: actual_value = CURL_LOCK_DATA_COOKIE; break;
                case 1: actual_value = CURL_LOCK_DATA_DNS; break;
                case 2: actual_value = CURL_LOCK_DATA_SSL_SESSION; break;
                default: actual_value = CURL_LOCK_DATA_COOKIE; break;
            }
            
            CURLSHcode res = curl_share_setopt(share_handle, option, actual_value);
            (void)res; /* Ignore result */
            break;
        }
        
        case 5: { /* curl_easy_ssls_export */
            CURLcode res = curl_easy_ssls_export(easy_handle, dummy_export_cb, NULL);
            (void)res; /* Ignore result */
            break;
        }
        
        default:
            break;
    }
    
    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_share_cleanup(share_handle);
    
    return 0;
}