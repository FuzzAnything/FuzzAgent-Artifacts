// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

/* Dummy lock/unlock functions for curl_share_setopt */
static void dummy_lock(CURL *handle, curl_lock_data data,
                      curl_lock_access access, void *userptr) {
    (void)handle;
    (void)data;
    (void)access;
    (void)userptr;
}

static void dummy_unlock(CURL *handle, curl_lock_data data,
                        void *userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
}

/* Callback for curl_easy_ssls_export with correct signature */
static CURLcode export_callback(void *userptr,
                                void *session_key,
                                const char *session_key_scheme,
                                const unsigned char *shmac,
                                size_t shmac_len,
                                const unsigned char *sdata,
                                size_t sdata_len,
                                long timeout,
                                int locked,
                                const char *scheme,
                                size_t scheme_len) {
    /* Minimal callback that does nothing */
    (void)userptr;
    (void)session_key;
    (void)session_key_scheme;
    (void)shmac;
    (void)shmac_len;
    (void)sdata;
    (void)sdata_len;
    (void)timeout;
    (void)locked;
    (void)scheme;
    (void)scheme_len;
    return CURLE_OK;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    CURL *easy_handle = NULL;
    CURLSH *share_handle = NULL;
    CURLcode easy_res;
    CURLSHcode share_res;
    const char *error_str;
    
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
    
    /* Set share options using fuzzer data - only use valid options */
    if (Size > 0) {
        /* Use only valid CURLSHoption values */
        CURLSHoption valid_options[] = {
            CURLSHOPT_SHARE,
            CURLSHOPT_UNSHARE,
            CURLSHOPT_LOCKFUNC,
            CURLSHOPT_UNLOCKFUNC,
            CURLSHOPT_USERDATA
        };
        int num_options = sizeof(valid_options) / sizeof(valid_options[0]);
        CURLSHoption option = valid_options[Data[0] % num_options];
        long param = 0;
        
        /* For LOCKFUNC/UNLOCKFUNC options, we need to provide valid function pointers */
        if (option == CURLSHOPT_LOCKFUNC || option == CURLSHOPT_UNLOCKFUNC) {
            if (option == CURLSHOPT_LOCKFUNC) {
                share_res = curl_share_setopt(share_handle, option, dummy_lock);
            } else {
                share_res = curl_share_setopt(share_handle, option, dummy_unlock);
            }
        } else {
            /* For other options, use a safe parameter */
            param = (long)(Size > 1 ? Data[1] : 0);
            share_res = curl_share_setopt(share_handle, option, param);
        }
        
        /* Get error string for share operation */
        error_str = curl_share_strerror(share_res);
        (void)error_str; /* Prevent unused variable warning */
    }
    
    /* Test curl_easy_ssls_import if we have enough data */
    if (Size >= 3) {
        const char *session_key = "test_session";
        /* Ensure we don't create pointers that go beyond Data buffer */
        size_t shmac_len = Size > 10 ? 10 : Size;
        const unsigned char *shmac = Data;
        
        /* Calculate sdata pointer safely */
        size_t sdata_offset = Size / 2;
        if (sdata_offset < Size) {
            const unsigned char *sdata = Data + sdata_offset;
            size_t sdata_len = Size - sdata_offset;
            
            /* Ensure sdata_len is reasonable */
            if (sdata_len > 0 && sdata_len <= Size) {
                easy_res = curl_easy_ssls_import(easy_handle, session_key,
                                                shmac, shmac_len,
                                                sdata, sdata_len);
                (void)easy_res; /* Prevent unused variable warning */
            }
        }
    }
    
    /* Test curl_easy_ssls_export */
    easy_res = curl_easy_ssls_export(easy_handle, export_callback, NULL);
    (void)easy_res; /* Prevent unused variable warning */
    
    /* Clean up share handle */
    share_res = curl_share_cleanup(share_handle);
    (void)share_res; /* Prevent unused variable warning */
    
    /* Clean up easy handle */
    curl_easy_cleanup(easy_handle);
    
    return 0;
}