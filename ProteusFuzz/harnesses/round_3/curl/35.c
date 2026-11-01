// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Required headers for curl API and internal types */
#include <curl/curl.h>
#include "mprintf.h"
#include "multi.h"
#include "typecheck-gcc.h"

/*
 * Helper function to create a null-terminated string from fuzz input.
 * This ensures safety for functions expecting C-strings.
 */
static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    /* Prevent empty inputs causing issues in string handling */
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare environment and data */
    char *safe_str = create_safe_string(Data, Size);
    if (!safe_str) {
        return 0;
    }

    /* 2. Fuzz curl_version()
     * No arguments needed. Returns static string. No cleanup needed.
     */
    const char *ver = curl_version();
    (void)ver; /* Use the result to avoid unused variable warnings */

    /* 3. Fuzz curl_getenv()
     * Takes a variable name. We use the fuzz input as the variable name.
     * Returns malloc'd string that must be curl_free()ed.
     */
    char *env_val = curl_getenv(safe_str);
    if (env_val) {
        curl_free(env_val);
    }

    /* 4. Fuzz curl_maprintf()
     * Format string needs to be safe. We use a fixed format string "%s"
     * and pass the fuzz input as the argument.
     * Note: The input is already null-terminated in safe_str.
     */
    char *fmt_result = curl_maprintf("%s", safe_str);
    if (fmt_result) {
        curl_free(fmt_result);
    }

    /* Try a format with integer to exercise different code paths */
    if (Size > 0) {
        int dummy_int = Data[0];
        char *fmt_result2 = curl_maprintf("Value: %d", dummy_int);
        if (fmt_result2) {
            curl_free(fmt_result2);
        }
    }

    /* 5. Fuzz curl_pushheader_bynum() and curl_pushheader_byname()
     * These require a struct curl_pushheaders.
     * Since 'struct curl_pushheaders' is an opaque type (forward declared only)
     * in the public headers, we cannot allocate or initialize it directly.
     * We test the API's robustness by passing NULL, which is a valid way to
     * trigger error handling paths for invalid struct arguments.
     */
    
    /* Test with NULL struct to ensure no crashes */
    char *null_header_num = curl_pushheader_bynum(NULL, 0);
    (void)null_header_num;
    
    char *null_header_name = curl_pushheader_byname(NULL, safe_str);
    (void)null_header_name;

    /* Test with a non-zero index on NULL struct */
    size_t index = (Size > 0) ? (size_t)Data[0] % 100 : 0;
    char *null_header_num_idx = curl_pushheader_bynum(NULL, index);
    (void)null_header_num_idx;

    /* 6. Cleanup */
    free(safe_str);

    return 0;
}