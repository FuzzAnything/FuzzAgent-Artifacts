// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static CURLcode dummy_export_callback(void *userptr, void *reserved,
                                      const char *session_key,
                                      const unsigned char *shmac, size_t shmac_len,
                                      const unsigned char *sdata, size_t sdata_len,
                                      long other1, int other2,
                                      const char *other3, size_t other4) {
    // Dummy callback that does nothing
    (void)userptr;
    (void)reserved;
    (void)session_key;
    (void)shmac;
    (void)shmac_len;
    (void)sdata;
    (void)sdata_len;
    (void)other1;
    (void)other2;
    (void)other3;
    (void)other4;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        return 0;
    }

    // Initialize share handle
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }

    // Use fuzzer data to determine which share option to set
    if (Size > 0) {
        CURLSHoption option;
        switch (Data[0] % 3) {
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
                break;
        }
        
        // Set share option with appropriate value based on option type
        if (option == CURLSHOPT_SHARE || option == CURLSHOPT_UNSHARE) {
            // Use a data type from CURL_LOCK_DATA_* enum
            CURLSHcode setopt_res = curl_share_setopt(share_handle, option, 
                                                     CURL_LOCK_DATA_COOKIE);
            (void)setopt_res; // Ignore result for fuzzing
        } else if (option == CURLSHOPT_USERDATA) {
            CURLSHcode setopt_res = curl_share_setopt(share_handle, option, 
                                                     (void*)0x1234);
            (void)setopt_res;
        }
    }

    // Try to export SSL sessions
    CURLcode export_res = curl_easy_ssls_export(easy_handle, 
                                               dummy_export_callback, 
                                               NULL);
    (void)export_res; // Ignore result for fuzzing

    // Try to import SSL sessions using fuzzer data
    if (Size > 0) {
        // Split fuzzer data for import parameters
        size_t session_key_len = Size > 10 ? 10 : Size;
        char *session_key = malloc(session_key_len + 1);
        if (session_key) {
            memcpy(session_key, Data, session_key_len);
            session_key[session_key_len] = '\0';
            
            // Use remaining data for shmac and sdata
            size_t remaining = Size > session_key_len ? Size - session_key_len : 0;
            const unsigned char *shmac = NULL;
            size_t shmac_len = 0;
            const unsigned char *sdata = NULL;
            size_t sdata_len = 0;
            
            if (remaining > 0) {
                shmac = Data + session_key_len;
                shmac_len = remaining / 2;
                sdata = shmac + shmac_len;
                sdata_len = remaining - shmac_len;
            }
            
            CURLcode import_res = curl_easy_ssls_import(easy_handle,
                                                       session_key,
                                                       shmac, shmac_len,
                                                       sdata, sdata_len);
            (void)import_res; // Ignore result for fuzzing
            
            free(session_key);
        }
    }

    // Clean up share handle
    CURLSHcode cleanup_res = curl_share_cleanup(share_handle);
    (void)cleanup_res; // Ignore result for fuzzing

    // Clean up easy handle
    curl_easy_cleanup(easy_handle);

    return 0;
}