// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"

/* Callback for curl_easy_ssls_export - corrected signature */
static CURLcode export_callback(void *userptr, void *ssl_ctx, const char *session_key,
                                const unsigned char *shmac, size_t shmac_len,
                                const unsigned char *sdata, size_t sdata_len,
                                long keytype, int keylen, const char *key, size_t flags) {
    /* Minimal callback that does nothing */
    (void)userptr;
    (void)ssl_ctx;
    (void)session_key;
    (void)shmac;
    (void)shmac_len;
    (void)sdata;
    (void)sdata_len;
    (void)keytype;
    (void)keylen;
    (void)key;
    (void)flags;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    CURL *easy_handle = NULL;
    CURLSH *share_handle = NULL;
    CURLcode easy_res;
    CURLSHcode share_res;
    
    /* Initialize easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        return 0;
    }
    
    /* Initialize share handle */
    share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }
    
    /* Use fuzzer data to set share options */
    if (Size >= sizeof(CURLSHoption)) {
        CURLSHoption option;
        long param;
        
        /* Use first bytes for option */
        memcpy(&option, Data, sizeof(CURLSHoption));
        option = option % (CURLSHOPT_LAST + 1);
        
        /* Use next bytes for parameter */
        if (Size >= sizeof(CURLSHoption) + sizeof(long)) {
            memcpy(&param, Data + sizeof(CURLSHoption), sizeof(long));
        } else {
            param = 0;
        }
        
        /* Try setting the option */
        curl_share_setopt(share_handle, option, param);
    }
    
    /* Associate share with easy handle */
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    
    /* Try SSL export with dummy callback */
    curl_easy_ssls_export(easy_handle, export_callback, NULL);
    
    /* Try SSL import with fuzzer data */
    if (Size > 0) {
        /* Split fuzzer data for import parameters */
        size_t key_len = Size > 32 ? 32 : Size;
        char *session_key = malloc(key_len + 1);
        if (session_key) {
            memcpy(session_key, Data, key_len);
            session_key[key_len] = '\0';
            
            size_t remaining = Size > key_len ? Size - key_len : 0;
            size_t shmac_len = remaining > 64 ? 64 : remaining;
            size_t sdata_len = remaining > shmac_len ? remaining - shmac_len : 0;
            
            const unsigned char *shmac = remaining > 0 ? Data + key_len : NULL;
            const unsigned char *sdata = remaining > shmac_len ? Data + key_len + shmac_len : NULL;
            
            curl_easy_ssls_import(easy_handle, session_key, shmac, shmac_len, sdata, sdata_len);
            
            free(session_key);
        }
    }
    
    /* Test share error string function with various codes */
    for (CURLSHcode code = CURLSHE_OK; code <= CURLSHE_LAST; code++) {
        const char *err_str = curl_share_strerror(code);
        (void)err_str; /* Prevent unused variable warning */
    }
    
    /* Test with some out-of-range codes */
    curl_share_strerror((CURLSHcode)(CURLSHE_LAST + 1));
    curl_share_strerror((CURLSHcode)(-1));
    
    /* Clean up share handle */
    share_res = curl_share_cleanup(share_handle);
    (void)share_res;
    
    /* Clean up easy handle */
    curl_easy_cleanup(easy_handle);
    
    return 0;
}