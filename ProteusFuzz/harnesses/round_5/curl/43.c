// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
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

/* 
 * Helper function to handle va_list operations for curl_mvsprintf
 * This function must be variadic to allow va_start
 */
static int test_mvsprintf(const char *format, ...) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    va_list args;
    va_start(args, format);
    
    int ret = curl_mvsprintf(buffer, format, args);
    
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    /* Guard against empty inputs */
    if (Size == 0) {
        return 0;
    }

    /* 
     * 1. curl_getenv
     * Retrieve an environment variable using a name derived from fuzz input.
     */
    {
        char env_name[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(env_name, Data, len);
        env_name[len] = '\0';
        
        char *result = curl_getenv(env_name);
        if (result != NULL) {
            curl_free(result);
        }
    }

    /* 
     * 2. curl_version
     * Get the library version string.
     */
    {
        const char *version = curl_version();
        (void)version; 
    }

    /* 
     * 3. curl_maprintf
     * Format a string using printf-style syntax.
     */
    {
        char *safe_input = malloc(Size + 1);
        if (safe_input == NULL) {
            return 0;
        }
        memcpy(safe_input, Data, Size);
        safe_input[Size] = '\0';

        char *formatted = curl_maprintf("%s", safe_input);
        if (formatted != NULL) {
            curl_free(formatted);
        }
        free(safe_input);
    }

    /* 
     * 4. curl_mvsprintf
     * Use the helper function to handle va_list correctly.
     */
    {
        char *safe_input = malloc(Size + 1);
        if (safe_input == NULL) {
            return 0;
        }
        memcpy(safe_input, Data, Size);
        safe_input[Size] = '\0';

        test_mvsprintf("%s", safe_input);
        free(safe_input);
    }

    /* 
     * 5. curl_global_sslset
     * Attempt to set global SSL backend.
     */
    {
        curl_ssl_backend **avail = NULL;
        
        char name_buf[64];
        size_t name_len = (Size < 63) ? Size : 63;
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        
        curl_global_sslset(CURLSSLBACKEND_NONE, name_buf, &avail);
        curl_global_sslset((curl_sslbackend)0, name_buf, &avail);
    }

    /* 
     * 6. curl_easy_setopt exercise
     * Initialize a handle and set a valid option to exercise the library.
     */
    {
        CURL *curl = curl_easy_init();
        if (curl != NULL) {
            char url_buf[256];
            size_t url_len = (Size < 255) ? Size : 255;
            memcpy(url_buf, Data, url_len);
            url_buf[url_len] = '\0';
            
            if (url_len > 0) {
                curl_easy_setopt(curl, CURLOPT_URL, url_buf);
            }
            
            curl_easy_cleanup(curl);
        }
    }

    return 0;
}