// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
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
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>

/* Helper callback for CURLOPT_RESOLVER_START_FUNCTION */
static int dummy_resolver_start_callback(void *userp, void *handle, void *data) {
    (void)userp;
    (void)handle;
    (void)data;
    return 0;
}

/* Helper callback for CURLOPT_READFUNCTION */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

/* Helper callback for CURLOPT_WRITEFUNCTION */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

/* Helper callback for CURLOPT_DEBUGFUNCTION */
static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    int msgs_in_queue = 0;
    CURLMsg *msg = NULL;
    char buffer[1024];
    long long_val;
    double double_val;
    struct curl_tlssessioninfo tls_info;
    struct curl_certinfo cert_info;
    curl_socket_t sock_val;
    curl_off_t off_val;
    char error_buffer[CURL_ERROR_SIZE];

    /* Initialize global curl */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create multi handle */
    multi_handle = curl_multi_init();

    /* Use input data to set various options safely */
    
    /* Set URL if data is large enough */
    if (Size >= 10) {
        char url[12];
        memcpy(url, Data, 10);
        url[10] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    }

    /* Set resolver start callback */
    curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_callback);
    if (Size > 0) {
        curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_DATA, (void*)(size_t)Data[0]);
    }

    /* Set read/write callbacks */
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    
    /* Only set data pointers if we have enough data to avoid out-of-bounds reads */
    if (Size > 1) {
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void*)(size_t)Data[1]);
    }
    if (Size > 2) {
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void*)(size_t)Data[2]);
    }

    /* Set debug callback */
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    if (Size > 3) {
        curl_easy_setopt(easy_handle, CURLOPT_DEBUGDATA, (void*)(size_t)Data[3]);
    }

    /* Set error buffer */
    curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buffer);

    /* Set long options safely */
    if (Size > 4) {
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)Data[4]);
    }
    if (Size > 5) {
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, (long)Data[5]);
    }
    if (Size > 6) {
        curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, (long)Data[6]);
    }

    /* Add to multi handle */
    if (multi_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
        
        /* Run multi perform */
        curl_multi_perform(multi_handle, &msgs_in_queue);
        
        /* Read messages */
        while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue)) != NULL) {
            (void)msg;
            /* Access msg data safely */
            if (msg->easy_handle) {
                /* Try to get info with various types */
                curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
                curl_easy_getinfo(msg->easy_handle, CURLINFO_SIZE_DOWNLOAD, &off_val);
                curl_easy_getinfo(msg->easy_handle, CURLINFO_HTTP_CODE, &long_val);
                
                /* Get TLS session info */
                struct curl_tlssessioninfo *tls_ptr = &tls_info;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_TLS_SESSION, &tls_ptr);
                
                /* Get cert info */
                struct curl_certinfo *cert_ptr = &cert_info;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_CERTINFO, &cert_ptr);
                
                /* Get socket info */
                curl_easy_getinfo(msg->easy_handle, CURLINFO_ACTIVESOCKET, &sock_val);
                
                /* Get string info */
                char *str_ptr = NULL;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_EFFECTIVE_URL, &str_ptr);
            }
        }
        
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }

    /* Test curl_msprintf */
    if (Size >= 10) {
        const char *format = "%s %d %c";
        char str1[64], str2[64];
        size_t len1 = (Size > 5) ? 5 : Size;
        size_t len2 = (Size > 5) ? (Size - 5 > 5 ? 5 : Size - 5) : 0;
        
        memcpy(str1, Data, len1);
        str1[len1] = '\0';
        
        if (len2 > 0) {
            memcpy(str2, Data + 5, len2);
            str2[len2] = '\0';
            int ret = curl_msprintf(buffer, format, str1, (int)Data[0], str2[0]);
            (void)ret;
        }
    }

    /* Test curl_mprintf */
    if (Size >= 3) {
        const char *format = "%d %d %d";
        int ret = curl_mprintf(format, (int)Data[0], (int)Data[1], (int)Data[2]);
        (void)ret;
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}