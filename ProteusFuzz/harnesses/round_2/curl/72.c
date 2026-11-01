// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Callback for curl_easy_ssls_export matching the expected signature */
/* 
 * Expected signature based on error:
 * CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, 
 *              const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 * Parameters:
 * 1. userptr
 * 2. session (void *)
 * 3. session_id (const char *)
 * 4. session_id_len (const unsigned char *) -> Actually this is usually a pointer to the length or the length itself?
 *    Looking at the error: const unsigned char *, unsigned long. 
 *    The error says: const char *, const unsigned char *, unsigned long.
 *    Let's re-read the error carefully:
 *    'CURLcode (void *, void *, const char *, const unsigned char *, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    vs
 *    'CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    
 *    My definition: 
 *    (void *userptr, void *session, const char *session_id, const unsigned char *session_id_len, const unsigned char *data, unsigned long data_len, ...)
 *    Wait, the error message shows the expected type has:
 *    ... const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long ...
 *    My definition had:
 *    ... const char *, const unsigned char *, const unsigned char *, unsigned long ...
 *    The order of (const unsigned char *, unsigned long) vs (const unsigned char *, unsigned long) seems swapped or I missed a parameter.
 *    
 *    Let's look at the standard curl_ssls_export_cb definition in curl.h:
 *    typedef CURLcode (*curl_ssls_export_cb)(void *userptr,
 *                                            void *session,
 *                                            const char *session_id,
 *                                            const unsigned char *session_id,
 *                                            unsigned long session_id_len,
 *                                            const unsigned char *data,
 *                                            unsigned long data_len,
 *                                            long age,
 *                                            int is_encrypted,
 *                                            const char *cipher_name,
 *                                            unsigned long cipher_name_len);
 *    Wait, the error message says:
 *    'CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    This maps to:
 *    1. void * (userptr)
 *    2. void * (session)
 *    3. const char * (session_id_str?)
 *    4. const unsigned char * (session_id_bytes)
 *    5. unsigned long (session_id_len)
 *    6. const unsigned char * (data)
 *    7. unsigned long (data_len)
 *    8. long (age)
 *    9. int (is_encrypted)
 *    10. const char * (cipher_name)
 *    11. unsigned long (cipher_name_len)
 *    
 *    My previous definition was:
 *    (void *userptr, void *session, const char *session_id, const unsigned char *session_id_len, const unsigned char *data, unsigned long data_len, ...)
 *    I had 10 parameters in my definition but the expected has 11.
 *    I missed the 'session_id_len' as a separate unsigned long, or I confused the types.
 *    The error message shows: const char *, const unsigned char *, unsigned long.
 *    My definition had: const char *, const unsigned char *, const unsigned char *, unsigned long.
 *    So I had an extra const unsigned char * or wrong type for session_id_len.
 *    
 *    Correct signature based on error:
 *    void *userptr
 *    void *session
 *    const char *session_id (maybe string rep?)
 *    const unsigned char *session_id (bytes)
 *    unsigned long session_id_len
 *    const unsigned char *data
 *    unsigned long data_len
 *    long age
 *    int is_encrypted
 *    const char *cipher_name
 *    unsigned long cipher_name_len
 *    
 *    Wait, the error says:
 *    'CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    My function:
 *    'CURLcode (void *, void *, const char *, const unsigned char *, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    Difference:
 *    Expected: ..., const unsigned char *, unsigned long, const unsigned char *, ...
 *    Mine:     ..., const unsigned char *, const unsigned char *, unsigned long, ...
 *    I swapped the order of 'unsigned long' and 'const unsigned char *' for the second pair?
 *    No, looking closely:
 *    Expected: 3:const char*, 4:const unsigned char*, 5:unsigned long, 6:const unsigned char*, 7:unsigned long
 *    Mine:     3:const char*, 4:const unsigned char*, 5:const unsigned char*, 6:unsigned long
 *    I am missing the 'unsigned long' after the first 'const unsigned char *'.
 *    So the correct signature is:
 *    void *userptr,
 *    void *session,
 *    const char *session_id,
 *    const unsigned char *session_id,
 *    unsigned long session_id_len,
 *    const unsigned char *data,
 *    unsigned long data_len,
 *    long age,
 *    int is_encrypted,
 *    const char *cipher_name,
 *    unsigned long cipher_name_len
 */
static CURLcode ssl_export_callback(void *userptr,
                                    void *session,
                                    const char *session_id_str,
                                    const unsigned char *session_id_bytes,
                                    unsigned long session_id_len,
                                    const unsigned char *data,
                                    unsigned long data_len,
                                    long age,
                                    int is_encrypted,
                                    const char *cipher_name,
                                    unsigned long cipher_name_len) {
    (void)userptr;
    (void)session;
    (void)session_id_str;
    (void)session_id_bytes;
    (void)session_id_len;
    (void)data;
    (void)data_len;
    (void)age;
    (void)is_encrypted;
    (void)cipher_name;
    (void)cipher_name_len;
    return CURLE_OK;
}

/* Simple write callback to satisfy curl requirements */
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)userdata;
    return size * nmemb;
}

/* Simple read callback to satisfy curl requirements */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)userdata;
    /* Return 0 to signal EOF immediately for fuzzing simplicity */
    return 0;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    long http_code = 0;
    double total_time = 0.0;
    curl_off_t download_size = 0;
    char error_buf[CURL_ERROR_SIZE];
    struct curl_slist *headers = NULL;
    struct curl_slist *info_slist = NULL;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlsinfo = NULL;
    curl_socket_t sock = CURL_SOCKET_BAD;
    const char *dummy_url = "http://example.com/dummy";
    const char *dummy_file = "./dummy_file";

    /* Initialize libcurl globally */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* Try to configure tracing with fuzz data if it looks like a string */
    if (Size > 0 && Data[Size - 1] == '\0') {
        curl_global_trace((const char *)Data);
    }

    /* Create a dummy file if needed */
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    /* Initialize easy handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Initialize error buffer */
    memset(error_buf, 0, sizeof(error_buf));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);

    /* Set a URL to allow perform to run without immediate failure due to missing URL */
    curl_easy_setopt(curl, CURLOPT_URL, dummy_url);
    
    /* Set basic callbacks to prevent crashes during perform */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "fuzzer-agent");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    /* Parse fuzz data to set various options to explore states */
    if (Size > 0) {
        /* Use first byte to decide which options to set */
        switch (Data[0] % 5) {
            case 0:
                /* Set a custom header */
                if (Size > 10) {
                    char *hdr = malloc(Size);
                    if (hdr) {
                        memcpy(hdr, Data, Size);
                        hdr[Size - 1] = '\0';
                        headers = curl_slist_append(headers, hdr);
                        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                    }
                }
                break;
            case 1:
                /* Set a custom range */
                if (Size > 4) {
                    char range[32];
                    snprintf(range, sizeof(range), "%ld-%ld", 
                             (long)Data[1], (long)Data[2]);
                    curl_easy_setopt(curl, CURLOPT_RANGE, range);
                }
                break;
            case 2:
                /* Set interface or other string options */
                if (Size > 5) {
                    char iface[32];
                    memset(iface, 0, sizeof(iface));
                    memcpy(iface, Data + 1, Size > 31 ? 31 : Size - 1);
                    curl_easy_setopt(curl, CURLOPT_INTERFACE, iface);
                }
                break;
            case 3:
                /* Set long options */
                if (Size > 2) {
                    long val = (long)Data[1] | ((long)Data[2] << 8);
                    curl_easy_setopt(curl, CURLOPT_VERBOSE, val % 2);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, val % 2);
                }
                break;
            case 4:
                /* Set custom request */
                if (Size > 5) {
                    char req[32];
                    memset(req, 0, sizeof(req));
                    memcpy(req, Data + 1, Size > 31 ? 31 : Size - 1);
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req);
                }
                break;
        }
    }

    /* 1. Invoke curl_easy_perform to ensure a transfer state exists */
    res = curl_easy_perform(curl);
    (void)res; /* Result is checked indirectly via getinfo */

    /* 2. Invoke curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res;

    /* 3. Invoke curl_easy_ssls_export */
    /* Note: This might return CURLE_NOT_BUILT_IN if SSL export is not compiled in, which is fine */
    res = curl_easy_ssls_export(curl, ssl_export_callback, NULL);
    (void)res;

    /* 4. Invoke curl_easy_getinfo with various types to test type checking and retrieval */
    
    /* Long type info */
    res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
    (void)res;

    /* Double type info */
    res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    (void)res;

    /* Off_t type info */
    res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_size);
    (void)res;

    /* Socket type info */
    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock);
    (void)res;

    /* String type info (e.g., content type) */
    char *content_type = NULL;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    (void)res;

    /* SList type info */
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &info_slist);
    (void)res;
    if (info_slist) {
        curl_slist_free_all(info_slist);
        info_slist = NULL;
    }

    /* Certinfo type info */
    res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    (void)res;
    if (certinfo) {
        /* curl_easy_getinfo does not free certinfo, we must free it if not NULL */
        /* Note: curl_certinfo_free is the correct function, but if not available in minimal env, 
           we skip explicit free to avoid compilation errors if struct is opaque. 
           In a full build, curl_certinfo_free(certinfo) should be called. */
        /* For safety in fuzzing without full headers, we just set to NULL. */
        certinfo = NULL;
    }

    /* TLS session info */
    res = curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlsinfo);
    (void)res;
    if (tlsinfo) {
        /* Similarly, freeing tlsinfo requires specific handling not always exposed in simple headers */
        tlsinfo = NULL;
    }

    /* Cleanup */
    if (headers) {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}