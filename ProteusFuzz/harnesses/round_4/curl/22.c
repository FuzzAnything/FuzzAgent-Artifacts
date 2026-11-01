// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>

/*
 * Fuzz driver for curl string manipulation and environment functions.
 * Targets: curl_maprintf, curl_getenv, curl_version, curl_msnprintf,
 *          curl_pushheader_byname, curl_mvsprintf.
 */

static void safe_free_string(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

/* Helper function to invoke curl_mvsprintf with variadic arguments */
static int invoke_mvsprintf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = curl_mvsprintf(buf, fmt, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test curl_version (no args, static return) */
    char *ver = curl_version();
    if (ver) {
        (void)strlen(ver);
    }

    /* Test curl_getenv */
    char env_var[256];
    size_t env_len = (Size > 255) ? 255 : Size;
    memcpy(env_var, Data, env_len);
    env_var[env_len] = '\0';
    
    /* Replace spaces with underscores to make it a valid variable name candidate */
    for (size_t i = 0; i < env_len; i++) {
        if (env_var[i] == ' ') env_var[i] = '_';
    }

    char *env_val = curl_getenv(env_var);
    if (env_val) {
        (void)strlen(env_val);
        safe_free_string(env_val);
    }

    /* Test curl_maprintf */
    char format_buf[128];
    size_t fmt_len = (Size > 127) ? 127 : Size;
    memcpy(format_buf, Data, fmt_len);
    format_buf[fmt_len] = '\0';

    char *mapped_str = NULL;
    
    /* Case 1: Format string only */
    mapped_str = curl_maprintf(format_buf);
    safe_free_string(mapped_str);

    /* Case 2: Format string with integer argument */
    if (Size > 0) {
        int val_int = (int)Data[0];
        mapped_str = curl_maprintf("%d %s", val_int, format_buf);
        safe_free_string(mapped_str);
    }

    /* Case 3: Format string with string argument */
    if (Size > 1) {
        char arg_str[64];
        size_t arg_len = (Size - 1 > 63) ? 63 : (Size - 1);
        memcpy(arg_str, Data + 1, arg_len);
        arg_str[arg_len] = '\0';
        mapped_str = curl_maprintf("Arg: %s", arg_str);
        safe_free_string(mapped_str);
    }

    /* Test curl_msnprintf */
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    
    int ret = curl_msnprintf(buffer, sizeof(buffer), format_buf);
    (void)ret;

    /* Test curl_msnprintf with arguments */
    if (Size > 0) {
        memset(buffer, 0, sizeof(buffer));
        int val = (int)Data[0];
        ret = curl_msnprintf(buffer, sizeof(buffer), "Value: %d, Len: %zu", val, Size);
        (void)ret;
    }

    /* Test curl_pushheader_byname */
    /* struct curl_pushheaders is an opaque type in public headers. 
       We cannot instantiate it directly. We test the NULL path and 
       rely on the internal implementation to handle invalid pointers. */
    struct curl_pushheaders *dummy_h_ptr = NULL;
    char *header_val = curl_pushheader_byname(dummy_h_ptr, "Content-Type");
    (void)header_val; /* Expect NULL */

    /* Test curl_mvsprintf via helper */
    /* Allocate a sufficiently large buffer to prevent overflow using standard malloc */
    char *safe_buffer = (char *)malloc(4096);
    if (safe_buffer) {
        memset(safe_buffer, 0, 4096);
        
        invoke_mvsprintf(safe_buffer, "Fuzz %s %d", format_buf, (int)Size);
        
        free(safe_buffer);
    }

    return 0;
}