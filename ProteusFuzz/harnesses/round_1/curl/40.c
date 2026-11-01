// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Required Header Files */
#include "curl.h"
#include "mprintf.h"
#include "multi.h"

/*
 * Helper to safely create a null-terminated string from fuzz input.
 * Returns a newly allocated string or NULL if size is 0.
 */
static char *make_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    char *str = (char *)malloc(size + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /*
     * 1. Prepare the environment
     * We will use the input data to drive different API calls.
     */

    /* Test curl_version() - No arguments needed, always safe */
    {
        char *ver = curl_version();
        if (ver) {
            /* Just use the string to ensure it's valid, no free needed */
            size_t len = strlen(ver);
            (void)len;
        }
    }

    /*
     * Test curl_msnprintf()
     * We need a buffer, a format string, and arguments.
     */
    {
        /* Ensure we have enough data for at least format and arg */
        if (Size < 2) {
            return 0;
        }

        size_t format_len = Size / 3;
        if (format_len == 0) format_len = 1;
        
        /* Calculate remaining size for argument */
        size_t remaining = Size - format_len;
        size_t arg_len = remaining;
        if (arg_len == 0) arg_len = 1;
        
        /* Safety check: ensure format_len + arg_len <= Size */
        if (format_len + arg_len > Size) {
            arg_len = Size - format_len;
            if (arg_len == 0) arg_len = 1;
        }

        /* Ensure we don't read past the buffer */
        if (format_len < Size) {
            char *arg = make_safe_string(Data + format_len, arg_len);
            
            if (arg) {
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                int ret = curl_msnprintf(buffer, sizeof(buffer), "%s", arg);
                (void)ret;
                curl_free(arg);
            }
        }
    }

    /*
     * Test curl_maprintf()
     * Similar to msnprintf but returns allocated memory.
     */
    {
        if (Size < 2) {
            return 0;
        }

        size_t format_len = Size / 3;
        if (format_len == 0) format_len = 1;
        
        size_t remaining = Size - format_len;
        size_t arg_len = remaining;
        if (arg_len == 0) arg_len = 1;

        if (format_len + arg_len > Size) {
            arg_len = Size - format_len;
            if (arg_len == 0) arg_len = 1;
        }

        if (format_len < Size) {
            char *arg = make_safe_string(Data + format_len, arg_len);
            
            if (arg) {
                char *result = curl_maprintf("%s", arg);
                if (result) {
                    size_t len = strlen(result);
                    (void)len;
                    curl_free(result);
                }
                curl_free(arg);
            }
        }
    }

    /*
     * Test curl_getenv()
     * We use a portion of the input as the variable name.
     */
    {
        size_t var_len = Size / 4;
        if (var_len == 0) var_len = 1;
        
        if (var_len <= Size) {
            char *var_name = make_safe_string(Data, var_len);
            if (var_name) {
                char *env_val = curl_getenv(var_name);
                if (env_val) {
                    size_t len = strlen(env_val);
                    (void)len;
                    curl_free(env_val);
                }
                curl_free(var_name);
            }
        }
    }

    /*
     * Test curl_pushheader_bynum() and curl_pushheader_byname()
     * These require a struct curl_pushheaders.
     * Since the struct definition is hidden (forward declaration only in public headers),
     * we cannot instantiate it directly. We must pass NULL to test error handling.
     */
    {
        /* Test with NULL struct pointer */
        char *h1 = curl_pushheader_bynum(NULL, 0);
        (void)h1;

        char *h2 = curl_pushheader_bynum(NULL, 1000);
        (void)h2;

        /* Test curl_pushheader_byname with a dummy name from input */
        size_t name_len = Size / 4;
        if (name_len == 0) name_len = 1;
        
        if (name_len <= Size) {
            char *name = make_safe_string(Data, name_len);
            if (name) {
                char *h3 = curl_pushheader_byname(NULL, name);
                (void)h3;
                curl_free(name);
            }
        }
    }

    /*
     * Test with file requirement (dummy_file)
     * Create a dummy file just in case.
     */
    {
        FILE *f = fopen("./dummy_file", "w");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }
    }

    return 0;
}