// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static CURLcode export_callback(void *userptr, void *session_data,
                                const char *session_key,
                                const unsigned char *shmac, size_t shmac_len,
                                const unsigned char *sdata, size_t sdata_len,
                                long validity, int flags,
                                const char *cipher, size_t cipher_len) {
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    CURLSH *share = curl_share_init();
    if (!share) {
        curl_easy_cleanup(easy);
        return 0;
    }

    curl_easy_setopt(easy, CURLOPT_SHARE, share);

    CURLSHcode share_res;
    share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    const char *error_str = curl_share_strerror(share_res);

    if (Size >= 1) {
        CURLSHoption option = (CURLSHoption)(Data[0] % 5);
        long param = (long)(Data[0]);
        curl_share_setopt(share, option, param);
    }

    curl_easy_ssls_export(easy, export_callback, NULL);

    if (Size >= 10) {
        const char *session_key = "test_session";
        size_t split = Size / 2;
        const unsigned char *shmac = Data;
        size_t shmac_len = split;
        const unsigned char *sdata = Data + split;
        size_t sdata_len = Size - split;
        curl_easy_ssls_import(easy, session_key, shmac, shmac_len, sdata, sdata_len);
    }

    curl_share_cleanup(share);
    curl_easy_cleanup(easy);

    return 0;
}