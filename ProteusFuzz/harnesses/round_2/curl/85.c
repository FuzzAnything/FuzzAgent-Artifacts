// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
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
#include <curl/mprintf.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_hstsread_callback(char *buf, size_t size, void *userdata) {
    (void)buf;
    (void)size;
    (void)userdata;
    return 0;
}

static int dummy_hstswrite_callback(const char *line, size_t len, void *userdata) {
    (void)line;
    (void)len;
    (void)userdata;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *ssl_ctx, void *userdata) {
    (void)curl;
    (void)ssl_ctx;
    (void)userdata;
    return 0;
}

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    FILE *dummy_file = fopen("./dummy_file", "w");
    if (!dummy_file) {
        curl_easy_cleanup(curl);
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    dummy_file = fopen("./dummy_file", "r");
    if (!dummy_file) {
        curl_easy_cleanup(curl);
        return 0;
    }

    // Fuzz curl_mfprintf
    if (Size >= 2) {
        char format_buf[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(format_buf, Data, fmt_len);
        format_buf[fmt_len] = '\0';
        curl_mfprintf(dummy_file, format_buf, Data[0]);
    }

    // Fuzz curl_easy_setopt with various callback types to trigger type checking logic
    // CURLOPT_HSTSREADFUNCTION
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, (curl_hstsread_callback)dummy_hstsread_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void *)Data);

    // CURLOPT_HSTSWRITEFUNCTION
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, (curl_hstswrite_callback)dummy_hstswrite_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, (void *)Data);

    // CURLOPT_SSL_CTX_FUNCTION
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, (curl_ssl_ctx_callback)dummy_ssl_ctx_callback);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, (void *)Data);

    // CURLOPT_READFUNCTION
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, (curl_read_callback)dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);

    // CURLOPT_WRITEFUNCTION
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (curl_write_callback)dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

    // Test with FILE* option (CURLOPT_INFILESIZE_LARGE is long, but let's try CURLOPT_RANDOM_FILE which is string)
    // Using a string from data for CURLOPT_RANDOM_FILE
    if (Size > 1) {
        char *random_file = (char *)malloc(Size + 1);
        if (random_file) {
            memcpy(random_file, Data, Size);
            random_file[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_RANDOM_FILE, random_file);
            free(random_file);
        }
    }

    // Test with long option
    if (Size >= 4) {
        long val = *(const long *)Data;
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, val);
    }

    // Test with string option
    if (Size > 1) {
        char *url = (char *)malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
            free(url);
        }
    }

    // Cleanup
    curl_easy_cleanup(curl);
    fclose(dummy_file);
    remove("./dummy_file");

    return 0;
}