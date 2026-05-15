// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

/* Dummy callback for curl_easy_ssls_export */
static CURLcode dummy_export_cb(void *userptr,
                                void *ssl_ctx,
                                const char *session_key,
                                const unsigned char *shmac,
                                size_t shmac_len,
                                const unsigned char *sdata,
                                size_t sdata_len,
                                long validity,
                                int flags,
                                const char *cipher,
                                size_t cipher_len) {
    /* Do nothing, just return success */
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    CURL *easy_handle = NULL;
    CURLSH *share_handle = NULL;
    CURLcode easy_res;
    CURLSHcode share_res;
    
    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    /* Create share handle */
    share_handle = curl_share_init();
    
    /* Test curl_share_setopt with various options using fuzzer data */
    if (share_handle) {
        /* Use first byte to determine option */
        if (Size > 0) {
            CURLSHoption option = (CURLSHoption)(Data[0] % 4); /* 0-3 */
            long param = (Size > 1) ? (long)Data[1] : 0L;
            
            share_res = curl_share_setopt(share_handle, option, param);
            
            /* Test error string function */
            const char *error_str = curl_share_strerror(share_res);
            (void)error_str; /* Use variable to avoid unused warning */
        }
        
        /* Set share handle to easy handle */
        curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    }
    
    /* Test curl_easy_ssls_export if we have enough data */
    if (Size >= 1) {
        easy_res = curl_easy_ssls_export(easy_handle, dummy_export_cb, NULL);
        (void)easy_res; /* Use variable to avoid unused warning */
    }
    
    /* Test curl_easy_ssls_import if we have enough data */
    if (Size >= 10) {
        /* Split fuzzer data for parameters */
        size_t offset = 0;
        
        /* Use first 4 bytes as session_key (null-terminated string) */
        char session_key[5] = {0};
        size_t key_len = (Size > 4) ? 4 : Size;
        memcpy(session_key, Data, key_len);
        session_key[key_len] = '\0';
        offset += key_len;
        
        /* Remaining data for shmac and sdata */
        size_t remaining = (Size > offset) ? Size - offset : 0;
        
        if (remaining >= 2) {
            /* Split remaining data between shmac and sdata */
            size_t shmac_len = remaining / 2;
            size_t sdata_len = remaining - shmac_len;
            
            const unsigned char *shmac = Data + offset;
            const unsigned char *sdata = Data + offset + shmac_len;
            
            easy_res = curl_easy_ssls_import(easy_handle, 
                                            session_key,
                                            shmac, shmac_len,
                                            sdata, sdata_len);
            (void)easy_res; /* Use variable to avoid unused warning */
        }
    }
    
    /* Clean up share handle */
    if (share_handle) {
        share_res = curl_share_cleanup(share_handle);
        (void)share_res; /* Use variable to avoid unused warning */
    }
    
    /* Clean up easy handle */
    curl_easy_cleanup(easy_handle);
    
    /* Clean up global state */
    curl_global_cleanup();
    
    return 0;
}