// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

/* Helper function to create a va_list for testing curl_mvaprintf */
static char *test_mvaprintf_wrapper(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char *res = curl_mvaprintf(fmt, ap);
    va_end(ap);
    return res;
}

/* Helper function to create a va_list for testing curl_mvsprintf */
static int test_mvsprintf_wrapper(char *buffer, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = curl_mvsprintf(buffer, fmt, ap);
    va_end(ap);
    return res;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null-termination for string operations */
    char *safe_input = (char *)malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    /* 1. Test curl_getenv */
    size_t env_name_len = (Size > 20) ? 20 : Size;
    char env_var[21];
    memcpy(env_var, safe_input, env_name_len);
    env_var[env_name_len] = '\0';
    
    char *env_result = curl_getenv(env_var);
    if (env_result) {
        (void)strlen(env_result);
        curl_free(env_result);
    }

    /* 2. Test curl_msnprintf */
    char buffer[256];
    int ret_msnprintf = curl_msnprintf(buffer, sizeof(buffer), "%s %d", safe_input, (int)(Size % 100));
    (void)ret_msnprintf;

    /* 3. Test curl_maprintf */
    char *maprintf_result = curl_maprintf("Input: %s, Size: %zu", safe_input, Size);
    if (maprintf_result) {
        (void)strlen(maprintf_result);
        curl_free(maprintf_result);
    }

    /* 4. Test curl_mvaprintf using helper */
    char *mvaprintf_result = test_mvaprintf_wrapper("%s", safe_input);
    if (mvaprintf_result) {
        (void)strlen(mvaprintf_result);
        curl_free(mvaprintf_result);
    }

    /* 5. Test curl_version */
    const char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    /* 6. Test curl_mvsprintf using helper */
    char buffer2[256];
    int ret_mvsprintf = test_mvsprintf_wrapper(buffer2, "Size: %zu, Data: %s", Size, safe_input);
    (void)ret_mvsprintf;

    free(safe_input);
    return 0;
}