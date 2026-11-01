// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
#include <stdio.h>

/* 
 * Fuzz driver for curl_share_init and related type-checking macros.
 * Since the Wcurl_* macros are compile-time warnings, we cannot "call" them directly.
 * Instead, we fuzz curl_share_init and curl_easy_setopt/getinfo with various inputs
 * to exercise the internal logic that these macros are designed to protect.
 */

static CURLSH *fuzz_share_init(const uint8_t *Data, size_t Size) {
    /* curl_share_init takes no arguments, but we can use fuzz data to influence behavior
     * indirectly (e.g., via environment or subsequent operations). For now, just call it. */
    return curl_share_init();
}

static void fuzz_share_cleanup(CURLSH *share) {
    if (share) {
        curl_share_cleanup(share);
    }
}

/* Dummy callback types to satisfy type-checking macros if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    return 0;
}

static int dummy_seek_callback(void *inclientp, curl_off_t offset, int origin) {
    return 0;
}

static int dummy_trailer_callback(void *clientp, char *key, char *value) {
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a share handle using fuzz input indirectly */
    CURLSH *share = fuzz_share_init(Data, Size);
    if (!share) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Associate the share handle with the CURL handle */
    curl_easy_setopt(curl, CURLOPT_SHARE, share);

    /* Use fuzz data to set various options, exercising type-checking macros indirectly */
    if (Size > 0) {
        /* Set URL from fuzz data (ensure null termination) */
        char *url_buf = malloc(Size + 1);
        if (url_buf) {
            memcpy(url_buf, Data, Size);
            url_buf[Size] = '\0';
            /* Skip if URL is empty or invalid */
            if (Size > 0 && url_buf[0] != '\0') {
                curl_easy_setopt(curl, CURLOPT_URL, url_buf);
            }
            free(url_buf);
        }
    }

    /* Set various options with fuzz data to trigger different code paths */
    if (Size > 0) {
        /* Set a string option */
        char *str_buf = malloc(Size + 1);
        if (str_buf) {
            memcpy(str_buf, Data, Size);
            str_buf[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
            free(str_buf);
        }
    }

    /* Set callback options */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);

    /* Set data pointers for callbacks */
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void *)Data);

    /* Perform a dummy request (will likely fail, but exercises the code path) */
    curl_easy_perform(curl);

    /* Get info to exercise curl_easy_getinfo type-checking */
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    char *url_str = NULL;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url_str);
    if (url_str) {
        /* url_str is owned by curl, do not free */
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    fuzz_share_cleanup(share);
    curl_global_cleanup();

    return 0;
}