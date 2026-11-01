// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Custom memory callbacks for curl_global_init_mem if needed */
static void *fuzz_malloc(size_t size) {
    return malloc(size);
}

static void fuzz_free(void *ptr) {
    free(ptr);
}

static void *fuzz_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *fuzz_strdup(const char *str) {
    return strdup(str);
}

static void *fuzz_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    long init_flags = 0;
    int use_custom_mem = 0;

    if (Size < 2) {
        return 0;
    }

    /* Use first byte to determine initialization method */
    if (Data[0] & 0x01) {
        use_custom_mem = 1;
    }

    /* Use second byte for init flags */
    init_flags = (long)(Data[1]);

    /* Initialize libcurl globally */
    if (use_custom_mem) {
        res = curl_global_init_mem(init_flags,
                                   fuzz_malloc,
                                   fuzz_free,
                                   fuzz_realloc,
                                   fuzz_strdup,
                                   fuzz_calloc);
    } else {
        res = curl_global_init(init_flags);
    }

    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a curl handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare data buffers from input */
    char *url_buf = NULL;
    char *post_buf = NULL;
    char *proxy_buf = NULL;

    /* Allocate and copy URL from input if possible */
    if (Size > 2) {
        size_t url_len = Size - 2;
        if (url_len > 1024) url_len = 1024;
        url_buf = malloc(url_len + 1);
        if (url_buf) {
            memcpy(url_buf, Data + 2, url_len);
            url_buf[url_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url_buf);
        }
    }

    /* Allocate and copy POST data if possible */
    if (Size > 4) {
        size_t post_len = Size - 4;
        if (post_len > 1024) post_len = 1024;
        post_buf = malloc(post_len + 1);
        if (post_buf) {
            memcpy(post_buf, Data + 4, post_len);
            post_buf[post_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf);
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        }
    }

    /* Set proxy if data available */
    if (Size > 6) {
        size_t proxy_len = Size - 6;
        if (proxy_len > 1024) proxy_len = 1024;
        proxy_buf = malloc(proxy_len + 1);
        if (proxy_buf) {
            memcpy(proxy_buf, Data + 6, proxy_len);
            proxy_buf[proxy_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_PROXY, proxy_buf);
        }
    }

    /* Set write callback */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    /* Perform the request */
    res = curl_easy_perform(curl);

    /* Perform upkeep */
    curl_easy_upkeep(curl);

    /* Cleanup buffers */
    free(url_buf);
    free(post_buf);
    free(proxy_buf);

    /* Cleanup curl handle */
    curl_easy_cleanup(curl);

    /* Cleanup global state */
    curl_global_cleanup();

    return 0;
}