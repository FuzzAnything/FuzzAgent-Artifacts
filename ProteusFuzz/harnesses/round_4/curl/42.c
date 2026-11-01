// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_version at version.c:145:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* 
 * Helper to safely extract a null-terminated string from fuzz input.
 * Returns a pointer to the input data (which is assumed to be const)
 * or NULL if no null terminator is found within Size.
 */
static const char *get_string_from_input(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    size_t i;
    for (i = 0; i < Size; i++) {
        if (Data[i] == '\0') {
            return (const char *)Data;
        }
    }
    /* If no null terminator, treat as invalid string for curl_getenv */
    return NULL;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    /* 1. Prepare the environment */
    
    /* Initialize curl globally to ensure internal state is ready */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* 
     * Target 1: curl_getenv
     * We try to extract a potential variable name from the input.
     * curl_getenv returns a malloc'd string that must be curl_free'd.
     * Note: curl_getenv may crash if passed NULL on some platforms/implementations.
     * We avoid passing NULL directly to prevent the observed SEGV.
     */
    const char *env_var = get_string_from_input(Data, Size);
    char *env_result = NULL;
    
    /* 
     * If we found a valid null-terminated string, pass it to curl_getenv.
     * If not, we pass an empty string to test edge cases safely.
     */
    if (env_var) {
        env_result = curl_getenv(env_var);
    } else {
        /* Test with empty string instead of NULL to avoid potential crashes */
        env_result = curl_getenv("");
    }

    /* Cleanup for curl_getenv result */
    if (env_result) {
        curl_free(env_result);
    }

    /* 
     * Target 2, 5, 6: Wcurl_easy_setopt_err_xferinfo_cb, etc.
     * These are compile-time macros (CURLWARNING). They do not have runtime logic
     * to fuzz directly in the same way as functions. They trigger compiler warnings
     * if types are mismatched. 
     * To exercise the code paths they guard, we can call curl_easy_setopt with
     * various options.
     */
    CURL *handle = curl_easy_init();
    if (handle) {
        /* Set a simple string option to exercise the option setting path */
        curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
        
        /* Set a long option */
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10L);
        
        /* Clean up handle */
        curl_easy_cleanup(handle);
    }

    /* 
     * Target 3: curl_version
     * Returns a static string. No cleanup needed.
     */
    const char *version = curl_version();
    (void)version; /* Use the return value to avoid unused warnings */

    /* 
     * Target 4: curl_global_sslset
     * Selects a global SSL backend. 
     * We try to call it with different IDs. 
     * Note: This should be called before curl_global_init usually, but we can
     * test its behavior after init or with various parameters.
     */
    curl_ssl_backend **avail = NULL;
    curl_ssl_backend **avail_ptr = &avail;
    
    /* Try with different backend IDs. We cast from input bytes to enum to explore states. */
    if (Size > 0) {
        curl_sslbackend id = (curl_sslbackend)Data[0];
        curl_global_sslset(id, NULL, avail_ptr);
    }
    
    /* Try with a name string if available in input */
    if (Size > 1) {
        /* Reuse env_var logic or just pass the raw data if it looks like a string */
        /* We already have env_var from earlier, but let's try a different slice if possible */
        if (Size > 2 && Data[1] != '\0') {
             /* Just try to pass the second byte as a name pointer if it's not null */
             /* This is a bit unsafe for a real string, but good for fuzzing null checks */
             curl_global_sslset(CURLSSLBACKEND_NONE, (const char *)Data + 1, avail_ptr);
        }
    }
    
    /* Try with NULL name and NULL avail */
    curl_global_sslset(CURLSSLBACKEND_NONE, NULL, NULL);

    /* 2. Cleanup */
    curl_global_cleanup();

    return 0;
}