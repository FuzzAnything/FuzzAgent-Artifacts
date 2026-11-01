// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl API functions */

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure input is null-terminated for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Test curl_msnprintf */
    char buffer[256];
    const char *format_str = "%s %d %f";
    int len = 0;
    if (Size >= 3) {
        int val_int = (int)Data[0];
        double val_double = (double)Data[1] / 10.0;
        len = curl_msnprintf(buffer, sizeof(buffer), format_str, input_str, val_int, val_double);
        (void)len; /* Suppress unused variable warning */
    } else {
        len = curl_msnprintf(buffer, sizeof(buffer), "%s", input_str);
        (void)len;
    }

    /* 2. Test curl_maprintf */
    char *allocated_str = curl_maprintf("Input: %s, Length: %zu", input_str, Size);
    if (allocated_str) {
        /* Use the string to ensure it's valid */
        (void)strlen(allocated_str);
        curl_free(allocated_str);
    }

    /* 3. Test curl_global_sslset */
    /* Fix: Ensure backend_name is a null-terminated string */
    curl_ssl_backend **avail = NULL;
    curl_sslbackend backend_id = (curl_sslbackend)(Data[0] % 10);
    
    const char *backend_name = "default";
    if (Size > 1) {
        /* Allocate a buffer for the name to ensure null termination */
        char *name_buf = (char *)malloc(Size);
        if (name_buf) {
            memcpy(name_buf, &Data[1], Size - 1);
            name_buf[Size - 1] = '\0'; /* Ensure null termination */
            backend_name = name_buf;
            /* Note: We do not free name_buf here to avoid double-free if we were 
               to reuse it, but since it's local scope, we must free it. 
               However, curl_global_sslset does not take ownership, so we free it now. */
            /* Wait, if we pass name_buf to curl_global_sslset, it just reads it. 
               We can free it immediately after the call or let it go out of scope. 
               Let's allocate, use, and free. */
        }
        
        CURLsslset result = curl_global_sslset(backend_id, backend_name, &avail);
        (void)result;

        if (name_buf) {
            free(name_buf);
        }
    } else {
        CURLsslset result = curl_global_sslset(backend_id, backend_name, &avail);
        (void)result;
    }

    /* 4. Test curl_version */
    char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    /* 5. Test curl_getenv */
    char *env_var = curl_getenv("PATH");
    if (env_var) {
        (void)strlen(env_var);
        curl_free(env_var);
    }

    /* Cleanup */
    free(input_str);

    return 0;
}