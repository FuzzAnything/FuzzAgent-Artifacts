// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

/* Dummy callback for curl_easy_ssls_export with correct signature */
static CURLcode dummy_export_callback(void *userptr, void *session_key,
                                      const char *shmac, const unsigned char *shmac_data,
                                      unsigned long shmac_len, const unsigned char *sdata,
                                      unsigned long sdata_len, long flags, int age,
                                      const char *cipher, unsigned long keylen) {
    return CURLE_OK;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    /* Initialize share handle */
    CURLSH *share = curl_share_init();
    if (!share) {
        return 0;
    }

    /* Initialize easy handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_share_cleanup(share);
        return 0;
    }

    /* Associate share with easy handle */
    curl_easy_setopt(easy, CURLOPT_SHARE, share);

    /* Fuzz curl_share_setopt with various options */
    if (Size > 0) {
        /* Use first byte to determine option */
        CURLSHoption option = (CURLSHoption)(Data[0] % 5); /* 5 common options */
        long param = (Size > 1) ? (long)Data[1] : 0L;
        
        curl_share_setopt(share, option, param);
    }

    /* Fuzz curl_easy_ssls_export */
    curl_easy_ssls_export(easy, dummy_export_callback, NULL);

    /* Fuzz curl_easy_ssls_import if we have enough data */
    if (Size >= 4) {
        size_t shmac_len = (Size > 8) ? 4 : Size / 4;
        size_t sdata_len = (Size > 8) ? Size - shmac_len : Size / 2;
        
        /* Ensure we don't overflow */
        if (shmac_len + sdata_len > Size) {
            shmac_len = Size / 2;
            sdata_len = Size - shmac_len;
        }
        
        const unsigned char *shmac = Data;
        const unsigned char *sdata = Data + shmac_len;
        
        curl_easy_ssls_import(easy, "fuzz_key", shmac, shmac_len, sdata, sdata_len);
    }

    /* Test curl_share_strerror with various error codes */
    CURLSHcode share_codes[] = {
        CURLSHE_OK,
        CURLSHE_BAD_OPTION,
        CURLSHE_IN_USE,
        CURLSHE_INVALID,
        CURLSHE_NOMEM,
        CURLSHE_NOT_BUILT_IN,
        CURLSHE_LAST
    };
    
    for (int i = 0; i < sizeof(share_codes)/sizeof(share_codes[0]); i++) {
        const char *error_str = curl_share_strerror(share_codes[i]);
        (void)error_str; /* Suppress unused variable warning */
    }

    /* Clean up - order matters! */
    curl_easy_cleanup(easy);
    
    /* Test curl_share_cleanup */
    CURLSHcode cleanup_res = curl_share_cleanup(share);
    (void)cleanup_res;

    return 0;
}