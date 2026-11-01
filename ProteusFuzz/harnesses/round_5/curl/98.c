// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback for CURLOPT_READFUNCTION */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userptr;
    return 0;
}

/* Dummy callback for CURLOPT_WRITEFUNCTION */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userptr;
    return size * nmemb;
}

/* 
 * Dummy callback for curl_easy_ssls_export.
 * Based on the error message, the signature is:
 * CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, 
 *              const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 */
static CURLcode dummy_ssls_export_cb(void *userptr, void *ssl_ctx, 
                                     const char *protocol, 
                                     const unsigned char *fingerprint, unsigned long fingerprint_len,
                                     const unsigned char *session_id, unsigned long session_id_len,
                                     long session_id_len_int, int session_type,
                                     const char *cipher, unsigned long cipher_len) {
    (void)userptr;
    (void)ssl_ctx;
    (void)protocol;
    (void)fingerprint;
    (void)fingerprint_len;
    (void)session_id;
    (void)session_id_len;
    (void)session_id_len_int;
    (void)session_type;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    /* Set a dummy URL to ensure perform has something to do, 
       but we will use a dummy file or local path to avoid network */
    const char *dummy_url = "file://./dummy_file";
    if (curl_easy_setopt(curl, CURLOPT_URL, dummy_url) != CURLE_OK) {
        /* If setting URL fails, we can still test other functions with invalid states */
    }

    /* Set read/write callbacks to prevent potential issues during perform */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

    /* Create a dummy file if needed for file:// protocol */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Fuzz curl_easy_perform */
    curl_easy_perform(curl);

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Fuzz curl_easy_ssls_export */
    /* We pass the input data as userptr to vary state */
    curl_easy_ssls_export(curl, dummy_ssls_export_cb, (void *)Data);

    /* Fuzz curl_easy_getinfo with various info types */
    /* We use the input data to select which info type to query */
    if (Size > 0) {
        int info_idx = Data[0] % 100; /* Limit to a safe range for demo */
        CURLINFO info = (CURLINFO)info_idx;
        
        /* Prepare variables for different info types */
        long long_val = 0;
        double double_val = 0.0;
        curl_off_t off_val = 0;
        char *string_val = NULL;
        curl_socket_t socket_val = 0;
        struct curl_slist *slist_val = NULL;
        struct curl_tlssessioninfo *tls_val = NULL;
        struct curl_certinfo *cert_val = NULL;

        /* Attempt to get info based on the type inferred or just try blindly */
        /* Note: curl_easy_getinfo is variadic, we must match types carefully */
        /* For fuzzing, we try a few common types based on the index */
        switch (info_idx % 6) {
            case 0:
                curl_easy_getinfo(curl, info, &long_val);
                break;
            case 1:
                curl_easy_getinfo(curl, info, &double_val);
                break;
            case 2:
                curl_easy_getinfo(curl, info, &off_val);
                break;
            case 3:
                curl_easy_getinfo(curl, info, &string_val);
                break;
            case 4:
                curl_easy_getinfo(curl, info, &socket_val);
                break;
            case 5:
                curl_easy_getinfo(curl, info, &slist_val);
                if (slist_val) {
                    curl_slist_free_all(slist_val);
                }
                break;
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}