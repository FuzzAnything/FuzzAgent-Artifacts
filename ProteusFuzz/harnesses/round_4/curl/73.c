// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Static helper to create a null-terminated string from fuzz input */
static char *create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a CURLSH handle */
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_share_setopt */
    /* We interpret the first byte as an option selector to diversify states */
    CURLSHoption share_opt = (CURLSHoption)(Data[0] & 0xFF);
    /* Try to set the option. We pass NULL or a dummy pointer for the value depending on option type.
       Since we don't know the exact type required by the fuzzer input, we pass NULL which is often safe or triggers specific paths.
       For options requiring a value, passing NULL might trigger an error path which is good for fuzzing. */
    curl_share_setopt(share, share_opt, NULL);

    /* Fuzz curl_share_strerror */
    /* Interpret a portion of data as an error code */
    CURLSHcode err_code = (CURLSHcode)(Data[0] ^ 0xFF);
    const char *err_str = curl_share_strerror(err_code);
    (void)err_str; /* Use the result to avoid unused variable warnings */

    /* Fuzz curl_easy_setopt with string options */
    /* Create a safe null-terminated string from input */
    char *safe_str = create_null_terminated_string(Data, Size);
    if (safe_str) {
        /* Fuzz various string-based options */
        curl_easy_setopt(curl, CURLOPT_URL, safe_str);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, safe_str);
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, safe_str);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, safe_str);
        
        /* Test with a specific option that might trigger the Wcurl_easy_setopt_err_interleave_cb warning path if misused,
           but here we use it correctly or with NULL to test error handling. */
        curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void *)safe_str);
        
        free(safe_str);
    }

    /* Fuzz curl_easy_setopt with long options */
    /* Ensure we have at least 2 bytes before accessing Data[1] */
    long long_val = 0;
    if (Size >= 2) {
        long_val = (long)(Data[0] << 8 | Data[1]);
    } else if (Size == 1) {
        long_val = (long)Data[0];
    }
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, long_val);

    /* Fuzz curl_easy_setopt with callback data options */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)(uintptr_t)Data);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)(uintptr_t)Data);

    /* Fuzz curl_easy_getinfo (to trigger type check macros like Wcurl_easy_getinfo_err_curl_socket) */
    curl_socket_t sock;
    long long_info;
    double double_info;
    char *string_info;
    
    /* These calls might fail or return CURLE_OK depending on state, but exercise the type checking logic */
    curl_easy_getinfo(curl, CURLINFO_SOCKET, &sock);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_info);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_info);

    /* Cleanup share */
    curl_share_cleanup(share);

    /* Cleanup curl handle */
    curl_easy_cleanup(curl);

    /* Cleanup global */
    curl_global_cleanup();

    return 0;
}