// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
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
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Custom memory callbacks for curl_global_init_mem */
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

/* Static flag to track initialization state */
static int initialized = 0;

/* Static flag to track if memory callbacks were used */
static int mem_init_used = 0;

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    long flags = 0;
    int use_mem_init = 0;

    /* Parse input to decide initialization strategy */
    if (Size > 0) {
        if (Data[0] & 1) {
            use_mem_init = 1;
        }
        if (Size > 1) {
            flags = (long)Data[1];
        }
    }

    /* Initialize libcurl if not already done */
    if (!initialized) {
        if (use_mem_init && !mem_init_used) {
            res = curl_global_init_mem(flags, fuzz_malloc, fuzz_free, fuzz_realloc, fuzz_strdup, fuzz_calloc);
            if (res == CURLE_OK) {
                initialized = 1;
                mem_init_used = 1;
            }
        } else {
            res = curl_global_init(flags);
            if (res == CURLE_OK) {
                initialized = 1;
            }
        }
    }

    /* Create a CURL handle */
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Set some options based on input data to explore different states */
    if (Size > 2) {
        char url[256];
        size_t url_len = (Size - 2 < 256) ? (Size - 2) : 256;
        memcpy(url, &Data[2], url_len);
        url[url_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    /* Set timeout to avoid hanging */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

    /* Set other options to trigger different code paths */
    if (Size > 3) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[3] & 1));
    }

    if (Size > 4) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(Data[4] & 1));
    }

    /* Perform the request */
    res = curl_easy_perform(curl);

    /* Call curl_easy_upkeep to test connection pool maintenance */
    curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);

    /* Cleanup global state if needed (optional, usually done at program exit) */
    /* curl_global_cleanup(); */

    return 0;
}