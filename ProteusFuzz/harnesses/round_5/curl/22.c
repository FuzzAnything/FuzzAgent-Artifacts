// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper to safely create a null-terminated string from fuzz input */
static char *make_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Test curl_version (no args, static string) */
    const char *ver = curl_version();
    (void)ver; /* Use the result to avoid unused warning, though it's static */

    /* Test curl_getenv with fuzz data as variable name */
    char *safe_var = make_safe_string(Data, Size);
    if (safe_var) {
        char *env_val = curl_getenv(safe_var);
        if (env_val) {
            curl_free(env_val);
        }
        free(safe_var);
    }

    /* Test curl_maprintf with fuzz data as format string and argument */
    /* Ensure we have a null-terminated string for format */
    char *safe_format = make_safe_string(Data, Size);
    if (safe_format) {
        /* Create a safe argument string as well */
        char *safe_arg = make_safe_string(Data, Size);
        if (safe_arg) {
            char *result = curl_maprintf("%s", safe_arg);
            if (result) {
                curl_free(result);
            }
            free(safe_arg);
        }
        free(safe_format);
    }

    /* Test curl_easy_setopt with various options to trigger type checking macros */
    /* We use safe strings for string options */
    char *url_str = make_safe_string(Data, Size);
    if (url_str) {
        curl_easy_setopt(curl, CURLOPT_URL, url_str);
        free(url_str);
    }

    /* Test with a long option */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)Size);

    /* Test with a callback option (using a dummy callback if needed, but here we just set NULL or valid types) */
    /* The Wcurl_* macros are compile-time checks, so calling curl_easy_setopt with correct types exercises them */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    /* Test curl_easy_getinfo with various info types */
    char *str_info;
    long long_info;
    double double_info;
    curl_off_t off_info;
    curl_socket_t sock_info;

    res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_info);
    (void)res; (void)str_info;

    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    (void)res; (void)long_info;

    res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &double_info);
    (void)res; (void)double_info;

    res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &off_info);
    (void)res; (void)off_info;

    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_info);
    (void)res; (void)sock_info;

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}