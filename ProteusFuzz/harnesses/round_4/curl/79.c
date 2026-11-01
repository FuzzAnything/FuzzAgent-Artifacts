// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_xferinfo_callback(CURL *handle, double dltotal, double dlnow, double ultotal, double ulnow, void *userdata) {
    (void)handle;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userdata;
    return 0;
}

/* Helper to safely call va_list functions using a variadic wrapper */
static void fuzz_mvprintf_helper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void fuzz_mvfprintf_helper(FILE *fp, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fp, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "w+");
    if (!fp) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Prepare a safe format string from input to avoid crashes in printf-like functions */
    char safe_format[256];
    size_t len = Size < 255 ? Size : 255;
    memcpy(safe_format, Data, len);
    safe_format[len] = '\0';
    
    /* Ensure the string is a valid format string by replacing % with %% if needed, 
       or just use %s with the data as argument to avoid format string vulnerabilities */
    /* Here we use a simple approach: print the data as a string */
    if (Size > 4 && Data[0] == 'p') {
        fuzz_mvprintf_helper("%s", safe_format);
    }

    if (Size > 4 && Data[0] == 'f') {
        fuzz_mvfprintf_helper(fp, "%s", safe_format);
    }

    /* Fuzz curl_easy_setopt with various callback types to trigger type checks */
    if (Size > 4 && Data[0] == 's') {
        /* Test CURLOPT_XFERINFOFUNCTION */
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void *)Data);

        /* Test CURLOPT_DEBUGFUNCTION */
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)Data);

        /* Test CURLOPT_WRITEFUNCTION */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

        /* Test CURLOPT_READFUNCTION */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);

        /* Test CURLOPT_URL */
        if (Size > 10) {
            char url[256];
            size_t url_len = Size < 255 ? Size : 255;
            memcpy(url, Data, url_len);
            url[url_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
        }
    }

    /* Fuzz curl_easy_getinfo to trigger type checks for curl_slist, etc. */
    if (Size > 4 && Data[0] == 'g') {
        char *str_ptr = NULL;
        long long_val = 0;
        double double_val = 0.0;
        struct curl_slist *slist_ptr = NULL;
        curl_off_t off_val = 0;
        curl_socket_t sock_val = CURL_SOCKET_BAD;

        curl_easy_getinfo(curl, CURLINFO_PRIVATE, &str_ptr);
        /* Use CURLINFO_RESPONSE_CODE instead of CURLINFO_HTTPCODE */
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_val);
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_ptr);
        curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_val);
        curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_val);

        if (slist_ptr) {
            curl_slist_free_all(slist_ptr);
        }
    }

    /* Cleanup */
    curl_slist_free_all(NULL);
    fclose(fp);
    curl_easy_cleanup(curl);

    return 0;
}