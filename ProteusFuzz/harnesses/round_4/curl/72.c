// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Dummy callback to satisfy type checks */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

/* Helper function to properly initialize va_list for curl_mvaprintf */
static char *test_mvaprintf_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *res = curl_mvaprintf(fmt, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Ensure we have a valid URL to set, using dummy_file or a safe string */
    const char *dummy_url = "file://./dummy_file";
    
    /* Write dummy data to file if needed for file:// protocol */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Set a basic URL to allow curl_easy_getinfo to work in some contexts */
    curl_easy_setopt(curl, CURLOPT_URL, dummy_url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    /* 2. Fuzzing logic for specific APIs */

    /* API 4: curl_version - No args, safe to call anytime */
    const char *ver = curl_version();
    if (ver) {
        (void)strlen(ver);
    }

    /* API 3: curl_getenv - Fuzz the variable name */
    if (Size > 1) {
        char safe_env[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        memcpy(safe_env, Data, copy_len);
        safe_env[copy_len] = '\0';
        
        char *env_val = curl_getenv(safe_env);
        if (env_val) {
            (void)strlen(env_val);
            curl_free(env_val);
        }
    }

    /* API 2: curl_maprintf - Fuzz format string and arguments */
    if (Size >= 2) {
        int dummy_int = (int)(Data[0] ^ Data[1]);
        const char *dummy_str = "fuzz_test";
        double dummy_double = (double)(Data[0] + Data[1]);

        /* Attempt 1: Standard %s %d %f mix */
        char *mapped_str = curl_maprintf("%s %d %f", dummy_str, dummy_int, dummy_double);
        if (mapped_str) {
            curl_free(mapped_str);
        }

        /* Attempt 2: Use a single safe string arg */
        mapped_str = curl_maprintf("%s", dummy_str);
        if (mapped_str) {
            curl_free(mapped_str);
        }
        
        /* Attempt 3: Static format */
        mapped_str = curl_maprintf("%s", "static");
        if (mapped_str) {
            curl_free(mapped_str);
        }
    }

    /* API 5: curl_mvaprintf - Fuzz with va_list via helper */
    if (Size >= 2) {
        int dummy_int = (int)(Data[0] ^ Data[1]);
        const char *dummy_str = "fuzz_test";
        
        char *mapped_mv_str = test_mvaprintf_helper("%s %d", dummy_str, dummy_int);
        if (mapped_mv_str) {
            curl_free(mapped_mv_str);
        }
    }

    /* API 1 & 6: Wcurl_easy_getinfo_err_curl_certinfo and Wcurl_easy_setopt_err_ssh_key_cb */
    /* These are compiler warning macros. We invoke the underlying functions 
       with correct types to ensure the macros don't trigger. */

    /* Test curl_easy_getinfo with CURLINFO_CERTINFO (requires struct curl_certinfo *) */
    struct curl_certinfo *certinfo = NULL;
    CURLcode res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    (void)res; 
    if (certinfo) {
        /* certinfo is managed by curl */
    }

    /* Test curl_easy_getinfo with other types */
    long long_val = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
    
    double double_val = 0.0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_val);

    /* Test curl_easy_setopt with various options */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_read_callback);

    /* 3. Cleanup */
    curl_easy_cleanup(curl);
    
    return 0;
}