// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static CURLcode dummy_export_cb(void *userptr,
                                void *ssl_ctx,
                                const char *session_key,
                                const unsigned char *shmac,
                                size_t shmac_len,
                                const unsigned char *sdata,
                                size_t sdata_len,
                                long timeout,
                                int permanent,
                                const char *scheme,
                                size_t port) {
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize share handle
    CURLSH *share = curl_share_init();
    if (!share) {
        return 0;
    }

    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_share_cleanup(share);
        return 0;
    }

    // Use fuzzer data to determine options and parameters
    uint8_t opt_selector = Data[0] % 6;
    size_t data_offset = 1;
    
    // Test curl_share_setopt with various options
    CURLSHcode share_res = CURLSHE_OK;
    switch (opt_selector) {
        case 0:
            share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
            break;
        case 1:
            share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
            break;
        case 2:
            share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
            break;
        case 3:
            share_res = curl_share_setopt(share, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_COOKIE);
            break;
        case 4:
            share_res = curl_share_setopt(share, CURLSHOPT_LOCKFUNC, NULL);
            break;
        case 5:
            share_res = curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, NULL);
            break;
    }

    // Test curl_share_strerror with the result
    const char *error_str = curl_share_strerror(share_res);

    // Test curl_easy_ssls_export with dummy callback
    if (Size > data_offset + 4) {
        CURLcode export_res = curl_easy_ssls_export(easy, dummy_export_cb, NULL);
        (void)export_res;
    }

    // Test curl_easy_ssls_import with fuzzer data
    if (Size > data_offset + 20) {
        const char *session_key = "fuzz_session";
        size_t shmac_len = Size > data_offset + 30 ? 16 : 0;
        size_t sdata_len = Size - data_offset - shmac_len;
        
        if (sdata_len > 0 && shmac_len > 0) {
            CURLcode import_res = curl_easy_ssls_import(easy,
                                                        session_key,
                                                        Data + data_offset,
                                                        shmac_len,
                                                        Data + data_offset + shmac_len,
                                                        sdata_len);
            (void)import_res;
        }
    }

    // Associate share with easy handle (using curl_easy_setopt which is commonly available)
    curl_easy_setopt(easy, CURLOPT_SHARE, share);

    // Clean up in reverse order
    curl_easy_cleanup(easy);
    
    // Test curl_share_cleanup
    CURLSHcode cleanup_res = curl_share_cleanup(share);
    (void)cleanup_res;

    return 0;
}