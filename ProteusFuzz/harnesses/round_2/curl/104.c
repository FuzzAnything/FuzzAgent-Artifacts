// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

/* 
 * The error "undefined reference to `LLVMFuzzerTestOneInput'" usually occurs when:
 * 1. The function is not defined in the file.
 * 2. The function is defined but with a different signature or visibility (e.g., static without proper linkage in some contexts, though static is usually fine for fuzzer).
 * 3. The file is not being compiled/linked correctly.
 * 
 * However, the user's error specifically points to the linker not finding the symbol.
 * In the previous attempt, the code looked correct. The issue might be subtle or related to how the fuzzer is invoked.
 * But strictly following the instructions: "Reply with only the content of the C source file".
 * 
 * Let's ensure the function is NOT static, as some fuzzer setups might have issues with static entry points if not handled by the wrapper correctly, 
 * although the instructions say "If defining any functions other than `LLVMFuzzerTestOneInput`, use `static` keyword".
 * The entry point `LLVMFuzzerTestOneInput` itself should generally be global.
 * 
 * Wait, the previous code DID have `static int LLVMFuzzerTestOneInput`. 
 * The standard libFuzzer entry point MUST be `extern "C"` in C++ or just `int` in C, and usually should NOT be static 
 * because the linker needs to find it from the object file to link with the fuzzer runtime's main.
 * If it is static, it has internal linkage and might not be exported to the symbol table in a way the linker expects for the main entry point.
 * 
 * Correction: The instructions say "If defining any functions other than `LLVMFuzzerTestOneInput`, use `static` keyword".
 * This implies `LLVMFuzzerTestOneInput` should NOT be static.
 * In the previous code, I wrote `static int LLVMFuzzerTestOneInput`. This was the mistake.
 * I will remove the `static` keyword from `LLVMFuzzerTestOneInput`.
 */

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize global curl environment
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    // Fuzz curl_global_trace
    // Ensure null termination for the config string
    size_t config_len = Size;
    if (config_len > 1024) config_len = 1024;
    char *config_str = (char *)malloc(config_len + 1);
    if (config_str) {
        memcpy(config_str, Data, config_len);
        config_str[config_len] = '\0';
        curl_global_trace(config_str);
        free(config_str);
    }

    // Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Fuzz curl_easy_upkeep with valid handle
    curl_easy_upkeep(curl);

    // Fuzz curl_easy_upkeep with NULL handle (should return CURLE_BAD_FUNCTION_ARGUMENT)
    curl_easy_upkeep(NULL);

    // Fuzz curl_easy_getinfo
    // We need to set a URL to make the handle somewhat valid for getinfo, though
    // most info is undefined until perform.
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    char *effective_url = NULL;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    if (effective_url) {
        // No need to free effective_url, it's managed by curl
    }

    // Perform a transfer to ensure state is valid for getinfo
    curl_easy_perform(curl);

    // Retry getinfo after perform
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    // Fuzz curl_mime_type
    // Create a mime structure
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Fuzz mimetype string
            size_t type_len = Size;
            if (type_len > 256) type_len = 256;
            char *mimetype_str = (char *)malloc(type_len + 1);
            if (mimetype_str) {
                memcpy(mimetype_str, Data, type_len);
                mimetype_str[type_len] = '\0';
                curl_mime_type(part, mimetype_str);
                free(mimetype_str);
            }

            // Test with NULL
            curl_mime_type(part, NULL);
        }
        curl_mime_free(mime);
    }

    // Test curl_mime_type with NULL part
    curl_mime_type(NULL, "text/plain");

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}