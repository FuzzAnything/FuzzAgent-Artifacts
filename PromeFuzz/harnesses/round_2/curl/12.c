// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* Callback function for curl_easy_ssls_export with correct signature */
static CURLcode export_callback(void *userptr, void *session_key, const char *session_id,
                                const unsigned char *shmac, unsigned long shmac_len,
                                const unsigned char *sdata, unsigned long sdata_len,
                                long validity, int flags, const char *cipher, unsigned long cipher_len) {
    /* Just return CURLE_OK to continue iteration */
    return CURLE_OK;
}

/* Helper function to create a dummy file with fuzzer data */
static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size > 1024 ? 1024 : size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size) {
    /* Initialize global curl state */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* Create share handle */
    CURLSH *share = curl_share_init();
    
    /* Create easy handle */
    CURL *easy = curl_easy_init();
    
    /* Test share functions if share was created */
    if (share) {
        /* Use fuzzer data to set share options */
        if (size > 0) {
            /* Try different share options based on input data */
            CURLSHoption options[] = {
                CURLSHOPT_SHARE,
                CURLSHOPT_UNSHARE
            };
            
            size_t num_options = sizeof(options) / sizeof(options[0]);
            if (num_options > 0) {
                CURLSHoption opt = options[data[0] % num_options];
                long param = 0;
                
                /* Set safe parameter values */
                if (opt == CURLSHOPT_SHARE || opt == CURLSHOPT_UNSHARE) {
                    long safe_params[] = {
                        CURL_LOCK_DATA_COOKIE,
                        CURL_LOCK_DATA_DNS,
                        CURL_LOCK_DATA_SSL_SESSION,
                        CURL_LOCK_DATA_CONNECT
                    };
                    size_t num_params = sizeof(safe_params) / sizeof(safe_params[0]);
                    if (num_params > 0) {
                        param = safe_params[(size > 1 ? data[1] : 0) % num_params];
                    }
                }
                
                CURLSHcode share_res = curl_share_setopt(share, opt, param);
                
                /* Test error string function */
                if (share_res != CURLSHE_OK) {
                    const char *err_str = curl_share_strerror(share_res);
                    (void)err_str; /* Prevent unused variable warning */
                }
            }
        }
        
        /* Clean up share handle */
        if (share) {
            CURLSHcode cleanup_res = curl_share_cleanup(share);
            (void)cleanup_res;
        }
    }
    
    /* Test SSL session import/export if we have an easy handle */
    if (easy) {
        /* Test SSL session import if we have enough data */
        if (size >= 10) {
            /* Create null-terminated session key */
            char session_key[9] = {0};
            size_t key_len = (data[0] % 7) + 1; /* 1-7 bytes plus null terminator */
            if (key_len > 8) key_len = 8;
            
            /* Copy session key data */
            if (key_len > 0 && size >= 2 + key_len) {
                memcpy(session_key, data + 2, key_len);
                session_key[key_len] = '\0';
                
                /* Calculate remaining lengths safely */
                size_t shmac_len = (data[1] % 8);
                if (shmac_len > 64) shmac_len = 64; /* Reasonable limit */
                
                size_t sdata_len = 0;
                if (size > 2 + key_len + shmac_len) {
                    sdata_len = size - (2 + key_len + shmac_len);
                    if (sdata_len > 1024) sdata_len = 1024; /* Reasonable limit */
                }
                
                /* Ensure we have valid pointers */
                const unsigned char *shmac = NULL;
                const unsigned char *sdata = NULL;
                
                if (shmac_len > 0 && size >= 2 + key_len + shmac_len) {
                    shmac = data + 2 + key_len;
                }
                
                if (sdata_len > 0 && size >= 2 + key_len + shmac_len) {
                    sdata = data + 2 + key_len + shmac_len;
                }
                
                /* Test import only if we have valid data */
                if (shmac_len > 0 && shmac != NULL) {
                    CURLcode import_res = curl_easy_ssls_import(easy, session_key, 
                                                               shmac, shmac_len,
                                                               sdata, sdata_len);
                    (void)import_res;
                }
            }
        }
        
        /* Test export */
        CURLcode export_res = curl_easy_ssls_export(easy, export_callback, NULL);
        (void)export_res;
        
        /* Clean up easy handle */
        curl_easy_cleanup(easy);
    }
    
    /* Create dummy file for any potential file operations */
    create_dummy_file(data, size);
    
    /* Remove dummy file */
    unlink("./dummy_file");
    
    /* Clean up global state */
    curl_global_cleanup();
    
    return 0;
}