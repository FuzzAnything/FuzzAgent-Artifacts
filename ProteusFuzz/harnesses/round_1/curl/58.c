// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

static int dummy_trailer_callback(CURL *curl, char *key, char *value, void *userptr) {
    (void)curl;
    (void)key;
    (void)value;
    (void)userptr;
    return 0;
}

static int dummy_chunk_bgn_callback(void *in, int bytesleft, void *out) {
    (void)in;
    (void)bytesleft;
    (void)out;
    return 0;
}

static int dummy_chunk_end_callback(void *in, void *out) {
    (void)in;
    (void)out;
    return 0;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Ensure we have a valid URL to prevent immediate failures in some modes
    // We use a dummy file or a safe string from input if possible
    if (Size > 10 && Data[0] != 0) {
        char url[256];
        size_t url_len = (Size < 255) ? Size : 255;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        // Ensure it looks like a URL or use a safe fallback
        if (strncmp(url, "http", 4) != 0 && strncmp(url, "ftp", 3) != 0) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:12345/fuzz");
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, url);
        }
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:12345/fuzz");
    }

    // Fuzz various options based on input data to trigger different code paths
    // We interpret the input bytes as option codes and values where possible

    // Set basic callbacks to avoid NULL pointer dereferences in internal logic
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);

    // Fuzz long options
    if (Size > 0) {
        long val_long = (long)(Data[0]);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, val_long);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, val_long);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, val_long);
    }

    // Fuzz string options (using input data as string, ensuring null termination)
    if (Size > 1) {
        char *str_val = (char *)malloc(Size + 1);
        if (str_val) {
            memcpy(str_val, Data, Size);
            str_val[Size] = '\0';
            
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_val);
            curl_easy_setopt(curl, CURLOPT_REFERER, str_val);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, str_val);
            curl_easy_setopt(curl, CURLOPT_RANGE, str_val);
            
            free(str_val);
        }
    }

    // Fuzz slist options
    if (Size > 2) {
        struct curl_slist *headers = NULL;
        char *str_val = (char *)malloc(Size + 1);
        if (str_val) {
            memcpy(str_val, Data, Size);
            str_val[Size] = '\0';
            headers = curl_slist_append(headers, str_val);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            free(str_val);
        }
        if (headers) {
            curl_slist_free_all(headers);
        }
    }

    // Fuzz POST fields
    if (Size > 3) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (void *)Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)Size);
    }

    // Fuzz getinfo calls with various types
    long l_out;
    double d_out;
    curl_off_t off_out;
    curl_socket_t sock_out;
    char *str_out = NULL;
    struct curl_slist *slist_out = NULL;
    struct curl_certinfo *cert_out = NULL;
    struct curl_tlssessioninfo *tls_out = NULL;

    // Trigger type checks for long
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &l_out);
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &l_out);
    curl_easy_getinfo(curl, CURLINFO_NUM_CONNECTS, &l_out);

    // Trigger type checks for double
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &d_out);
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &d_out);

    // Trigger type checks for curl_off_t
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_out);
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_out);

    // Trigger type checks for curl_socket_t
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_out);

    // Trigger type checks for string
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_out);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &str_out);

    // Trigger type checks for slist
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_out);
    if (slist_out) curl_slist_free_all(slist_out);

    // Trigger type checks for certinfo
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_out);
    if (cert_out) {
        // certinfo is complex, just ensure we don't crash accessing it
        // No need to free manually as curl handles it in some versions, 
        // but standard practice is to free if we own it. 
        // For fuzzing, we just want to exercise the getter.
    }

    // Trigger type checks for tlssessioninfo
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_out);

    // Exercise curl_easy_reset to clear state and reuse handle
    curl_easy_reset(curl);

    // Re-apply some options after reset to test state transitions
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)Data[0]);
    }

    // Cleanup
    curl_easy_cleanup(curl);

    return 0;
}