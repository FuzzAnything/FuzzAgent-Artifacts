// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

/* Correct callback function for curl_easy_ssls_export with proper signature */
static CURLcode export_callback(void *userptr, void *session_key,
                                const char *session_key_str,
                                const unsigned char *shmac, unsigned long shmac_len,
                                const unsigned char *sdata, unsigned long sdata_len,
                                long validity, int flags,
                                const char *cipher, unsigned long cipher_len) {
    /* Minimal callback that does nothing */
    return CURLE_OK;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    /* Initialize share and easy handles */
    CURLSH *share = curl_share_init();
    CURL *easy = curl_easy_init();
    
    if (!share || !easy) {
        if (easy) curl_easy_cleanup(easy);
        if (share) curl_share_cleanup(share);
        return 0;
    }
    
    /* 1. Test curl_share_setopt with various options - use valid enum values */
    if (Size > 0) {
        /* Use valid CURLSHoption values to avoid undefined behavior */
        CURLSHoption options[] = {
            CURLSHOPT_SHARE,
            CURLSHOPT_UNSHARE,
            CURLSHOPT_LOCKFUNC,
            CURLSHOPT_UNLOCKFUNC,
            CURLSHOPT_USERDATA
        };
        
        size_t option_count = sizeof(options) / sizeof(options[0]);
        CURLSHoption option = options[Data[0] % option_count];
        
        /* For LOCKFUNC/UNLOCKFUNC options, pass NULL callbacks */
        if (option == CURLSHOPT_LOCKFUNC || option == CURLSHOPT_UNLOCKFUNC) {
            curl_share_setopt(share, option, NULL);
        } else if (option == CURLSHOPT_USERDATA) {
            curl_share_setopt(share, option, NULL);
        } else {
            /* For SHARE/UNSHARE, use valid CURL_LOCK_DATA values */
            long param = (long)(CURL_LOCK_DATA_COOKIE + (Data[0] % 5));
            curl_share_setopt(share, option, param);
        }
        
        /* Test error handling */
        CURLSHcode share_code = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        const char *error_str = curl_share_strerror(share_code);
    }
    
    /* 2. Test curl_easy_ssls_import if we have enough data */
    if (Size >= 3) {
        /* Create a session key string */
        const char *session_key = "fuzz_session";
        
        /* Ensure we don't read beyond Data buffer */
        size_t max_offset = Size > 10 ? 10 : Size;
        size_t shmac_len = Data[0] % max_offset;
        size_t sdata_len = Data[1] % (Size - shmac_len);
        
        /* Ensure lengths don't exceed buffer bounds */
        if (shmac_len + sdata_len <= Size - 2) {
            const unsigned char *shmac = Data + 2;
            const unsigned char *sdata = Data + 2 + shmac_len;
            
            curl_easy_ssls_import(easy, session_key, shmac, shmac_len, sdata, sdata_len);
        }
    }
    
    /* 3. Test curl_easy_ssls_export */
    curl_easy_ssls_export(easy, export_callback, NULL);
    
    /* 4. Test curl_share_strerror with valid CURLSHcode range */
    for (int i = 0; i <= 5; i++) {
        curl_share_strerror((CURLSHcode)i);
    }
    
    /* 5. Clean up handles */
    curl_easy_cleanup(easy);
    curl_share_cleanup(share);
    
    return 0;
}