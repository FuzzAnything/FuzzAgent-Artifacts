// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"

/* Dummy callback for curl_easy_ssls_export with correct signature */
static CURLcode dummy_export_callback(void *userptr, void *handle,
                                      const char *session_key,
                                      const unsigned char *shmac, unsigned long shmac_len,
                                      const unsigned char *sdata, unsigned long sdata_len,
                                      long arg7, int arg8,
                                      const char *arg9, unsigned long arg10) {
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    /* Initialize curl globally */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* Create easy handle */
    CURL *easy_handle = curl_easy_init();
    
    /* Create share handle */
    CURLSH *share_handle = curl_share_init();
    
    /* Fuzz curl_share_setopt with safe options only */
    if (share_handle && Size > 0) {
        /* Use safe options that don't require function pointers */
        CURLSHoption option;
        int opt_selector = Data[0] % 3;
        
        switch(opt_selector) {
            case 0:
                option = CURLSHOPT_SHARE;
                break;
            case 1:
                option = CURLSHOPT_UNSHARE;
                break;
            case 2:
                option = CURLSHOPT_USERDATA;
                break;
            default:
                option = CURLSHOPT_SHARE;
        }
        
        long param = 0;
        if (Size > 1) {
            param = (long)Data[1];
        }
        
        /* Try setting the option */
        CURLSHcode setopt_result = curl_share_setopt(share_handle, option, param);
        
        /* Get error string for the result */
        const char *error_str = curl_share_strerror(setopt_result);
        (void)error_str;
    }
    
    /* Fuzz curl_easy_ssls_import with bounds checking */
    if (easy_handle && Size >= 5) {
        /* Split input data safely */
        size_t offset = 0;
        
        /* Use first 4 bytes for session_key (as hex string) */
        char session_key[9] = {0};
        for (int i = 0; i < 4 && offset < Size; i++) {
            snprintf(&session_key[i*2], 3, "%02x", Data[offset++]);
        }
        
        /* Next byte for shmac_len, ensure it doesn't exceed available data */
        size_t shmac_len = 0;
        if (offset < Size) {
            shmac_len = Data[offset++] % 32; /* Limit to reasonable size */
        }
        
        /* Ensure shmac_len doesn't exceed remaining data */
        if (shmac_len > Size - offset) {
            shmac_len = Size - offset;
        }
        
        /* Get shmac pointer if we have data */
        const unsigned char *shmac = NULL;
        if (shmac_len > 0 && offset < Size) {
            shmac = Data + offset;
            offset += shmac_len;
        }
        
        /* Remaining data for sdata with bounds checking */
        size_t sdata_len = 0;
        const unsigned char *sdata = NULL;
        if (offset < Size) {
            sdata_len = Size - offset;
            sdata = Data + offset;
        }
        
        /* Call import function only if we have valid pointers */
        if (shmac != NULL || sdata != NULL) {
            curl_easy_ssls_import(easy_handle, session_key, 
                                 shmac, shmac_len, sdata, sdata_len);
        }
    }
    
    /* Fuzz curl_easy_ssls_export */
    if (easy_handle) {
        curl_easy_ssls_export(easy_handle, dummy_export_callback, NULL);
    }
    
    /* Fuzz curl_share_strerror with valid error codes */
    if (Size > 0) {
        /* Use modulo to ensure we stay within reasonable range */
        CURLSHcode error_code = (CURLSHcode)(Data[0] % 8);
        const char *error_msg = curl_share_strerror(error_code);
        (void)error_msg;
    }
    
    /* Clean up share handle */
    if (share_handle) {
        CURLSHcode cleanup_result = curl_share_cleanup(share_handle);
        (void)cleanup_result;
    }
    
    /* Clean up easy handle */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    curl_global_cleanup();
    return 0;
}