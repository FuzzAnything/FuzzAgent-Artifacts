// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    if (size > 1024) {
        size = 1024;
    }
    char *str = malloc(size + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

/* Wrapper to test curl_mvsprintf properly with a variadic function */
static int wrapper_mvsprintf(const char *format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    int res = curl_mvsprintf(buf, format, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. curl_version: No args, static string, no cleanup needed */
    char *ver = curl_version();
    if (ver) {
        (void)strlen(ver);
    }

    /* 2. curl_getenv: Test with a safe key derived from input */
    char env_key[33]; /* Increased size to accommodate null terminator safely */
    size_t key_len = (Size > 32) ? 32 : Size;
    memcpy(env_key, Data, key_len);
    env_key[key_len] = '\0';
    
    /* Ensure env_key is a valid identifier */
    for (size_t i = 0; i < key_len; i++) {
        unsigned char c = (unsigned char)env_key[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || c == '_')) {
            env_key[i] = 'a';
        }
    }

    char *env_val = curl_getenv(env_key);
    if (env_val) {
        (void)strlen(env_val);
        curl_free(env_val);
    }

    /* 3. curl_msnprintf: Safe formatting */
    char buffer[256];
    char fmt_buf[64];
    size_t fmt_len = (Size > 63) ? 63 : Size;
    memcpy(fmt_buf, Data, fmt_len);
    fmt_buf[fmt_len] = '\0';
    
    char *safe_arg = create_safe_string(Data, Size);
    
    int ret_snprintf = curl_msnprintf(buffer, sizeof(buffer), fmt_buf, safe_arg ? safe_arg : "");
    (void)ret_snprintf;
    
    if (safe_arg) {
        free(safe_arg);
    }

    /* 4. curl_maprintf: Dynamic allocation, must free */
    char *fmt_buf2 = create_safe_string(Data, Size);
    size_t offset = (Size > 10) ? 10 : 0;
    size_t remaining = (Size > 10) ? Size - 10 : 0;
    char *safe_arg2 = create_safe_string(Data + offset, remaining);
    
    char *mapped_str = curl_maprintf(fmt_buf2 ? fmt_buf2 : "test", safe_arg2 ? safe_arg2 : "");
    if (mapped_str) {
        (void)strlen(mapped_str);
        curl_free(mapped_str);
    }
    
    if (fmt_buf2) free(fmt_buf2);
    if (safe_arg2) free(safe_arg2);

    /* 5. curl_mvsprintf: Use the wrapper defined above */
    char *fmt_buf3 = create_safe_string(Data, Size);
    char *safe_arg3 = create_safe_string(Data, Size);
    
    int ret_mv = wrapper_mvsprintf(fmt_buf3 ? fmt_buf3 : "test", safe_arg3 ? safe_arg3 : "");
    (void)ret_mv;

    if (fmt_buf3) free(fmt_buf3);
    if (safe_arg3) free(safe_arg3);

    /* 6. curl_global_sslset: Global state, call once or handle failure */
    curl_ssl_backend **avail = NULL;
    char name_buf[33]; /* Increased size to accommodate null terminator safely */
    size_t name_len = (Size > 32) ? 32 : Size;
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';
    
    /* Sanitize name to avoid null bytes in the middle */
    for (size_t i = 0; i < name_len; i++) {
        if (name_buf[i] == '\0') name_buf[i] = 'x';
    }

    curl_sslbackend test_ids[] = { CURLSSLBACKEND_NONE, CURLSSLBACKEND_OPENSSL, CURLSSLBACKEND_GNUTLS };
    for (int i = 0; i < 3; i++) {
        (void)curl_global_sslset(test_ids[i], name_buf, &avail);
    }

    return 0;
}