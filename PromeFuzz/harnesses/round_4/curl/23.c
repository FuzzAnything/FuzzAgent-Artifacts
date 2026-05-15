// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"

static CURLcode dummy_export_callback(void *userptr, void *handle,
                                      const char *session_key,
                                      const unsigned char *shmac, size_t shmac_len,
                                      const unsigned char *sdata, size_t sdata_len,
                                      long something1, int something2,
                                      const char *something3, size_t something4) {
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        return 0;
    }

    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }

    uint8_t selector = Data[0] % 6;
    size_t offset = 1;

    switch (selector) {
        case 0: {
            if (Size - offset < 1) break;
            size_t key_len = Data[offset] % 16;
            offset++;
            if (Size - offset < key_len) break;
            
            char session_key[17] = {0};
            if (key_len > 0) {
                memcpy(session_key, Data + offset, key_len);
                session_key[key_len] = '\0';
            } else {
                session_key[0] = '\0';
            }
            offset += key_len;

            size_t shmac_len = 0;
            size_t sdata_len = 0;
            const unsigned char *shmac = NULL;
            const unsigned char *sdata = NULL;

            if (Size - offset >= 1) {
                shmac_len = Data[offset] % 32;
                offset++;
                if (Size - offset >= shmac_len) {
                    shmac = Data + offset;
                    offset += shmac_len;
                } else {
                    shmac_len = 0;
                }
            }

            if (Size - offset >= 1) {
                sdata_len = Data[offset] % 64;
                offset++;
                if (Size - offset >= sdata_len) {
                    sdata = Data + offset;
                } else {
                    sdata_len = 0;
                }
            }

            curl_easy_ssls_import(easy_handle, session_key, shmac, shmac_len, sdata, sdata_len);
            break;
        }
        case 1: {
            if (Size - offset < 1) break;
            CURLSHcode error_code = (CURLSHcode)(Data[offset] % 10);
            const char *error_str = curl_share_strerror(error_code);
            (void)error_str;
            break;
        }
        case 2: {
            CURLSHcode cleanup_result = curl_share_cleanup(share_handle);
            share_handle = NULL;
            (void)cleanup_result;
            break;
        }
        case 3: {
            CURLSH *new_share = curl_share_init();
            if (new_share) {
                CURLSHcode cleanup_result = curl_share_cleanup(new_share);
                (void)cleanup_result;
            }
            break;
        }
        case 4: {
            if (Size - offset < 2) break;
            CURLSHoption option = (CURLSHoption)(Data[offset] % 5);
            offset++;
            long param = (long)Data[offset];
            
            curl_share_setopt(share_handle, option, param);
            break;
        }
        case 5: {
            curl_easy_ssls_export(easy_handle, dummy_export_callback, NULL);
            break;
        }
    }

    if (share_handle) {
        CURLSHcode cleanup_result = curl_share_cleanup(share_handle);
        (void)cleanup_result;
    }
    curl_easy_cleanup(easy_handle);

    return 0;
}