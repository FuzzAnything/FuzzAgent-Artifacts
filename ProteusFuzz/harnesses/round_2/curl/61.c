// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback functions to satisfy type requirements */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_debug_cb(CURL *curl, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)curl; (void)type; (void)data; (void)size; (void)userdata;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_hstsread_cb(void *userdata, char *url, char *contents, size_t size) {
    (void)userdata; (void)url; (void)contents; (void)size;
    return 0;
}

static int dummy_resolver_start_cb(void *userdata, const char *hostname, int family, int flags, curl_resolver_start_callback cb, void *cbp) {
    (void)userdata; (void)hostname; (void)family; (void)flags; (void)cb; (void)cbp;
    return 0;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Initialize error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));

    /* Create a safe string buffer from fuzz input for URL and POST fields */
    char *safe_str = NULL;
    if (Size > 0) {
        safe_str = (char *)malloc(Size + 1);
        if (safe_str) {
            memcpy(safe_str, Data, Size);
            safe_str[Size] = '\0'; /* Ensure null termination */
        }
    }

    /* Set URL if we have a valid string */
    if (safe_str && strlen(safe_str) > 0) {
        curl_easy_setopt(curl, CURLOPT_URL, safe_str);
    } else {
        /* Fallback URL if input is empty or invalid */
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/dummy");
    }

    /* Set various options to exercise different code paths and type checks */
    
    /* String options */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, safe_str ? safe_str : "FuzzerAgent");
    curl_easy_setopt(curl, CURLOPT_REFERER, safe_str ? safe_str : "http://example.com/ref");
    
    /* Callback options */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
    
    /* Data pointers for callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)0x1234);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)0x5678);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)0x9ABC);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)0xDEF0);
    curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void *)0x1111);

    /* Long options */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);

    /* POST fields */
    if (safe_str) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, safe_str);
    }

    /* Error buffer */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* Perform the request (non-blocking or with timeout to avoid hanging) */
    /* Note: We don't actually expect a network connection, just exercising the API logic */
    (void)curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    free(safe_str);

    return 0;
}