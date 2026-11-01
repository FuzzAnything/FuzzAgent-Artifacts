// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void safe_fuzz_string(char *out, const uint8_t *data, size_t size) {
    if (size == 0) {
        out[0] = '\0';
        return;
    }
    size_t len = size;
    if (len > 1024) len = 1024;
    memcpy(out, data, len);
    out[len] = '\0';
}

static int test_mvsprintf_helper(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Test curl_version */
    (void)curl_version();

    /* Test curl_getenv with various inputs */
    char env_var[256];
    safe_fuzz_string(env_var, Data, Size < 256 ? Size : 256);
    char *env_result = curl_getenv(env_var);
    if (env_result) {
        curl_free(env_result);
    }

    /* Test curl_msnprintf */
    char buffer[1024];
    char arg_str[256];
    
    safe_fuzz_string(arg_str, Data, Size < 256 ? Size : 256);

    /* Safe format usage: avoid %s with untrusted data unless null-terminated */
    int ret = curl_msnprintf(buffer, sizeof(buffer), "%s", arg_str);
    (void)ret;

    /* Test with integer format */
    if (Size > 1) {
        int val = Data[0] + Data[1];
        ret = curl_msnprintf(buffer, sizeof(buffer), "%d", val);
        (void)ret;
    }

    /* Test curl_maprintf with safe formats */
    char *maprintf_result = NULL;
    if (Size > 2) {
        int a = Data[0];
        int b = Data[1];
        maprintf_result = curl_maprintf("%d %d", a, b);
    } else {
        maprintf_result = curl_maprintf("static");
    }
    if (maprintf_result) {
        curl_free(maprintf_result);
        maprintf_result = NULL;
    }

    /* Test curl_mvsprintf */
    if (Size > 2) {
        int a = Data[0];
        int b = Data[1];
        test_mvsprintf_helper(buffer, "%d %d", a, b);
    }

    /* Test curl_global_sslset */
    curl_ssl_backend **avail = NULL;
    curl_ssl_backend *avail_list[2] = {NULL, NULL};
    avail = avail_list;
    
    /* Try with ID 0 (default) and NULL name */
    curl_global_sslset((curl_sslbackend)0, NULL, &avail);
    
    /* Try with a name from fuzz data */
    safe_fuzz_string(arg_str, Data, Size < 256 ? Size : 256);
    curl_global_sslset((curl_sslbackend)0, arg_str, &avail);

    return 0;
}