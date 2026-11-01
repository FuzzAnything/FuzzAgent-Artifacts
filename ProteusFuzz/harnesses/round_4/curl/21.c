// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Helper function to test va_list based functions */
static void test_va_functions(const char *fmt, const char *arg) {
    char *result_mvaprintf = curl_maprintf(fmt, arg);
    if (result_mvaprintf) {
        size_t len = strlen(result_mvaprintf);
        if (len > 0) {
            result_mvaprintf[len - 1] = 'Z';
        }
        curl_free(result_mvaprintf);
    }

    char buffer_mvsprintf[2048]; 
    int ret_mvsprintf = curl_msnprintf(buffer_mvsprintf, sizeof(buffer_mvsprintf), fmt, arg);
    (void)ret_mvsprintf;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for string operations */
    char *safe_data = malloc(Size + 1);
    if (!safe_data) {
        return 0;
    }
    memcpy(safe_data, Data, Size);
    safe_data[Size] = '\0';

    /* 1. Test curl_maprintf with various formats from input */
    char *result_maprintf = NULL;
    const char *fmt = safe_data;
    
    /* Basic test with the input as format string and a dummy argument */
    result_maprintf = curl_maprintf(fmt, safe_data);
    if (result_maprintf) {
        size_t len = strlen(result_maprintf);
        if (len > 0) {
            result_maprintf[len - 1] = 'X';
        }
        curl_free(result_maprintf);
    }

    /* 2. Test curl_msnprintf */
    char buffer[1024];
    int ret_msnprintf = curl_msnprintf(buffer, sizeof(buffer), fmt, safe_data);
    (void)ret_msnprintf;

    /* 3. Test curl_getenv */
    char *env_var = curl_getenv(safe_data);
    if (env_var) {
        size_t env_len = strlen(env_var);
        if (env_len > 0) {
            env_var[0] = 'Y';
        }
        curl_free(env_var);
    }

    /* 4. Test curl_mvaprintf and curl_mvsprintf via helper (using curl_maprintf internally) */
    test_va_functions(fmt, safe_data);

    /* 5. Test curl_easy_setopt type checking macros indirectly */
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, safe_data);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "FuzzAgent");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
        curl_easy_cleanup(curl);
    }

    free(safe_data);
    return 0;
}