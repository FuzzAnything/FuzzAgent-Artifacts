// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Helper function to test curl_mvsprintf with a valid va_list */
static int test_mvsprintf_helper(const char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    
    int ret = curl_mvsprintf(buffer, format, args);
    
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a safe null-terminated string from fuzz input */
    char *safe_input = NULL;
    char *safe_input2 = NULL;
    
    if (Size > 0) {
        safe_input = (char *)malloc(Size + 1);
        if (!safe_input) {
            curl_global_cleanup();
            return 0;
        }
        memcpy(safe_input, Data, Size);
        safe_input[Size] = '\0';
        
        /* Create a second copy for functions needing string args */
        safe_input2 = (char *)malloc(Size + 1);
        if (!safe_input2) {
            free(safe_input);
            curl_global_cleanup();
            return 0;
        }
        memcpy(safe_input2, Data, Size);
        safe_input2[Size] = '\0';
    }

    char *escaped = NULL;
    char *unescaped = NULL;
    char *mapped = NULL;
    char buffer[256];
    int ret;

    /* 1. Test curl_escape */
    if (safe_input) {
        escaped = curl_escape(safe_input, (int)Size);
        if (escaped) {
            curl_free(escaped);
        }
    }

    /* 2. Test curl_unescape */
    if (safe_input) {
        unescaped = curl_unescape(safe_input, (int)Size);
        if (unescaped) {
            curl_free(unescaped);
        }
    }

    /* 3. Test curl_maprintf with %s format */
    if (safe_input2) {
        /* Ensure format string is safe */
        mapped = curl_maprintf("prefix_%s_suffix", safe_input2);
        if (mapped) {
            curl_free(mapped);
        }
    }

    /* 4. Test curl_msnprintf */
    if (safe_input2) {
        ret = curl_msnprintf(buffer, sizeof(buffer), "format: %s", safe_input2);
        (void)ret;
    }

    /* 5. Wcurl_multi_setopt_err_long is a compile-time warning macro, 
       not a runtime function. We skip runtime invocation. */

    /* 6. Test curl_mvsprintf via the helper function */
    if (safe_input2) {
        /* Use the helper to correctly handle va_list */
        ret = test_mvsprintf_helper("input: %s", safe_input2);
        (void)ret;
    }

    /* Cleanup */
    free(safe_input);
    free(safe_input2);
    curl_global_cleanup();

    return 0;
}