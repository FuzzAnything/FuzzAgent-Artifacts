// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void safe_free(void *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

/* Helper function to test curl_mvsprintf with variable arguments */
static int test_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test curl_version */
    char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    /* Test curl_getenv with a controlled variable name derived from input */
    char env_var_name[64];
    size_t env_name_len = Size < sizeof(env_var_name) - 1 ? Size : sizeof(env_var_name) - 1;
    memcpy(env_var_name, Data, env_name_len);
    env_var_name[env_name_len] = '\0';
    
    /* Ensure env_var_name does not contain null bytes in the middle by replacing them */
    for (size_t i = 0; i < env_name_len; i++) {
        if (env_var_name[i] == '\0') {
            env_var_name[i] = 'x';
        }
    }
    
    char *env_val = curl_getenv(env_var_name);
    if (env_val) {
        (void)strlen(env_val);
        safe_free(env_val);
    }

    /* Test curl_global_sslset */
    curl_ssl_backend **avail = NULL;
    curl_sslbackend id = CURLSSLBACKEND_NONE;
    const char *name = NULL;
    
    if (Size >= 2) {
        id = (curl_sslbackend)Data[0];
        if (Data[1] != 0) {
            name = (const char *)Data + 1;
        }
    }
    curl_global_sslset(id, name, &avail);

    /* Test curl_maprintf with safe format strings */
    char *fmt1 = "%d %d %d";
    int arg1 = 0, arg2 = 0, arg3 = 0;
    if (Size >= 3) {
        arg1 = Data[0];
        arg2 = Data[1];
        arg3 = Data[2];
    }
    char *res1 = curl_maprintf(fmt1, arg1, arg2, arg3);
    if (res1) {
        safe_free(res1);
    }

    /* Test curl_maprintf with string argument */
    char *fmt2 = "%s %s";
    char str1[32], str2[32];
    /* Ensure we leave room for the null terminator */
    size_t s1_len = Size < 31 ? Size : 31;
    size_t s2_len = Size < 31 ? Size : 31;
    
    memcpy(str1, Data, s1_len);
    str1[s1_len] = '\0';
    
    memcpy(str2, Data, s2_len);
    str2[s2_len] = '\0';
    
    /* Ensure no null bytes in the middle for %s safety */
    for (size_t i = 0; i < s1_len; i++) if (str1[i] == '\0') str1[i] = 'x';
    for (size_t i = 0; i < s2_len; i++) if (str2[i] == '\0') str2[i] = 'x';

    char *res2 = curl_maprintf(fmt2, str1, str2);
    if (res2) {
        safe_free(res2);
    }

    /* Test curl_msnprintf */
    char buffer[256];
    int ret;
    
    /* Format with integers */
    ret = curl_msnprintf(buffer, sizeof(buffer), "%d %d", arg1, arg2);
    (void)ret;

    /* Format with strings */
    ret = curl_msnprintf(buffer, sizeof(buffer), "%s %s", str1, str2);
    (void)ret;

    /* Format with length specifier for safety if input was raw */
    if (Size >= 2) {
        ret = curl_msnprintf(buffer, sizeof(buffer), "%.*s %.*s", 
                             (int)s1_len, str1, (int)s2_len, str2);
        (void)ret;
    }

    /* Test curl_mvsprintf via helper */
    {
        char buf_mvs[256];
        int r = test_mvsprintf(buf_mvs, "%d %d", arg1, arg2);
        (void)r;
    }

    return 0;
}