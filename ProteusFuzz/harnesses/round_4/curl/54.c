// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* 
 * Fuzz driver for curl API functions.
 * Targets: curl_global_trace, curl_easy_strerror, curl_mime_type.
 * Note: Wcurl_* macros are compile-time warnings, not runtime functions.
 */

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl globally once per process (safe for fuzzing context) */
    static int initialized = 0;
    if (!initialized) {
        CURLcode init_res = curl_global_init(CURL_GLOBAL_ALL);
        if (init_res != CURLE_OK) {
            return 0;
        }
        initialized = 1;
    }

    /* 
     * 1. Fuzz curl_global_trace
     * The function takes a config string. We ensure it is null-terminated.
     * We create a temporary buffer to hold the null-terminated string.
     */
    char *config_str = NULL;
    if (Size > 0) {
        config_str = (char *)malloc(Size + 1);
        if (config_str) {
            memcpy(config_str, Data, Size);
            config_str[Size] = '\0';
            
            /* Invoke curl_global_trace with the fuzzed string */
            curl_global_trace(config_str);
            
            /* Also try with NULL to cover that path */
            curl_global_trace(NULL);
            
            free(config_str);
        }
    }

    /* 
     * 2. Fuzz curl_easy_strerror
     * We iterate through possible CURLcode values derived from the input.
     * CURLcode is typically an enum, so we cast input bytes to it.
     */
    for (size_t i = 0; i < Size; i++) {
        CURLcode code = (CURLcode)Data[i];
        const char *str = curl_easy_strerror(code);
        (void)str; /* Use the result to avoid unused warnings */
    }
    
    /* Also test specific known error codes */
    (void)curl_easy_strerror(CURLE_OK);
    (void)curl_easy_strerror(CURLE_COULDNT_CONNECT);
    (void)curl_easy_strerror(CURLE_OUT_OF_MEMORY);

    /* 
     * 3. Fuzz curl_mime_type
     * Requires a valid curl_mimepart. We create a mime structure and a part.
     * If creation fails, we skip this part of the fuzzing.
     */
    curl_mime *mime = curl_mime_init(NULL);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Prepare a null-terminated mimetype string from input */
            char *mimetype = NULL;
            if (Size > 0) {
                mimetype = (char *)malloc(Size + 1);
                if (mimetype) {
                    memcpy(mimetype, Data, Size);
                    mimetype[Size] = '\0';
                    
                    /* Invoke curl_mime_type with fuzzed string */
                    curl_mime_type(part, mimetype);
                    
                    /* Invoke with NULL to clear type */
                    curl_mime_type(part, NULL);
                    
                    /* Invoke with empty string */
                    curl_mime_type(part, "");
                    
                    free(mimetype);
                }
            }
            
            /* Test with NULL part (should return CURLE_BAD_FUNCTION_ARGUMENT) */
            curl_mime_type(NULL, "text/plain");
        }
        curl_mime_free(mime);
    }

    /* 
     * Cleanup
     * curl_global_cleanup is typically called once at program exit.
     * In a fuzzing loop, we might want to avoid calling it every iteration 
     * if it's expensive, but for safety and state reset, we can call it 
     * only if we are sure we don't need curl anymore in this process.
     * However, standard practice for fuzzers is to init once and cleanup at exit.
     * We will NOT call curl_global_cleanup here to avoid overhead and potential 
     * issues with the static 'initialized' flag logic if the fuzzer re-enters.
     * The static initialization handles the global state.
     */

    return 0;
}