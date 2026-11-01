// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Dummy read callback for mime data */
static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userp) {
    size_t total = size * nitems;
    if (total > 0 && userp) {
        /* Return some data based on input size to avoid infinite loops */
        size_t len = (size_t)((size_t)userp % 1024);
        if (len > total) len = total;
        memcpy(buffer, userp, len);
        return len;
    }
    return CURL_READFUNC_ABORT;
}

/* Dummy seek callback for mime data */
static int dummy_seek_callback(void *userp, curl_off_t offset, int origin) {
    return CURL_SEEKFUNC_OK;
}

/* Dummy free callback for mime data */
static void dummy_free_callback(void *userp) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl globally once per process (thread-safe check not needed for fuzzer) */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to configure curl options diversely */
    /* Set URL from input data if it looks like a URL or just use a dummy */
    char url_buf[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';
    
    /* Ensure URL is valid enough for the API call, fallback to dummy */
    if (url_len < 5 || strstr(url_buf, "http") == NULL) {
        strcpy(url_buf, "http://example.com/dummy");
    }
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);

    /* Set other options based on input bytes to explore states */
    if (Size > 0) {
        long timeout = (long)Data[0];
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }
    if (Size > 1) {
        long connect_timeout = (long)Data[1];
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    }
    if (Size > 2) {
        /* Use a dummy file path for options expecting a file */
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
    }
    if (Size > 3) {
        /* Set a custom header */
        char header[64];
        size_t h_len = (Size < 63) ? Size : 63;
        memcpy(header, Data, h_len);
        header[h_len] = '\0';
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    /* Test curl_easy_perform */
    curl_easy_perform(curl);

    /* Test curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Test curl_easy_getinfo with various types */
    char *str_ptr = NULL;
    long long_val = 0;
    double double_val = 0.0;
    curl_off_t off_val = 0;
    curl_socket_t sock_val = CURL_SOCKET_BAD;
    struct curl_slist *slist_ptr = NULL;
    struct curl_certinfo *certinfo_ptr = NULL;
    struct curl_tlssessioninfo *tls_ptr = NULL;

    /* Trigger type checking macros implicitly by calling getinfo */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_ptr);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &off_val);
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_val);
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_ptr);
    if (slist_ptr) curl_slist_free_all(slist_ptr);
    
    /* Test MIME API */
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Use input data as argument for callback, ensuring bounds check */
            void *cb_arg = NULL;
            if (Size > 2) {
                cb_arg = (void *)(uintptr_t)(Data[2]);
            }
            curl_off_t data_size = 0;
            if (Size > 3) {
                data_size = (curl_off_t)Data[3];
            }
            
            /* Test curl_mime_data_cb */
            curl_mime_data_cb(part, data_size, dummy_read_callback, 
                              dummy_seek_callback, dummy_free_callback, cb_arg);
            
            /* Set the mime data for the request */
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        }
        curl_mime_free(mime);
    }

    /* Perform again with MIME data if set */
    curl_easy_perform(curl);
    curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}