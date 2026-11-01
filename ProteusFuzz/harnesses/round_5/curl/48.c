// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Required headers for curl API and internal types */
#include "curl.h"
#include "multi.h"
#include "mprintf.h"

/*
 * Static helper to safely create a null-terminated string from fuzz input.
 * This prevents passing raw binary data to functions expecting valid C-strings.
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

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    /* 1. Prepare the environment */
    
    /* Test curl_version (no args, static string) */
    char *ver = curl_version();
    if (ver) {
        /* Just access it to ensure it's valid, no cleanup needed for static string */
        (void)ver[0];
    }

    /* Test curl_getenv */
    /* We try to parse the input as a potential environment variable name */
    if (Size > 0) {
        char *env_name = create_safe_string(Data, Size);
        if (env_name) {
            char *env_val = curl_getenv(env_name);
            if (env_val) {
                /* Use the value briefly */
                (void)env_val[0];
                curl_free(env_val);
            }
            free(env_name);
        }
    }

    /* Test curl_maprintf */
    /* We need a valid format string and valid arguments */
    if (Size > 0) {
        /* Create a safe format string from input, but sanitize % to avoid crashes if possible,
         * or just let the library handle invalid format strings gracefully.
         * To ensure we test the '%s' case safely as per instructions:
         */
        char *safe_input = create_safe_string(Data, Size);
        if (safe_input) {
            /* Test with a simple format that uses the input as a string */
            char *result = curl_maprintf("Input: %s", safe_input);
            if (result) {
                (void)result[0];
                curl_free(result);
            }
            
            /* Test with a fixed format and input as string */
            result = curl_maprintf("Data: %s", safe_input);
            if (result) {
                curl_free(result);
            }
            
            free(safe_input);
        }
    } else {
        /* Fallback for empty input */
        char *result = curl_maprintf("Empty");
        if (result) {
            curl_free(result);
        }
    }

    /* Test curl_msnprintf */
    char buffer[256];
    if (Size > 0) {
        char *safe_input = create_safe_string(Data, Size);
        if (safe_input) {
            int ret = curl_msnprintf(buffer, sizeof(buffer), "Msg: %s", safe_input);
            (void)ret;
            free(safe_input);
        }
    } else {
        int ret = curl_msnprintf(buffer, sizeof(buffer), "No data");
        (void)ret;
    }

    /* Test curl_pushheader_bynum and curl_pushheader_byname */
    /* These functions require a valid struct curl_pushheaders.
     * Since struct curl_pushheaders is opaque (forward declared only) in multi.h,
     * we cannot instantiate it directly. We test with NULL and a pointer to uninitialized memory
     * to ensure the functions handle invalid inputs gracefully (returning NULL).
     */
    
    /* Case 1: NULL struct */
    char *h1 = curl_pushheader_bynum(NULL, 0);
    (void)h1; /* Should be NULL */
    
    char *h2 = curl_pushheader_byname(NULL, "Content-Type");
    (void)h2; /* Should be NULL */

    /* Case 2: Invalid/Uninitialized struct (simulated) */
    /* We allocate memory for the struct but do not initialize internal fields,
     * simulating an invalid handle passed to the API.
     * Since the struct is incomplete, we cannot use sizeof. We allocate a fixed small size
     * sufficient to hold a pointer or a few pointers, which is typical for opaque structs.
     * The actual size is unknown, but passing a non-NULL pointer to a valid memory region
     * is sufficient to trigger internal checks in the library.
     */
    size_t dummy_struct_size = 64; /* Heuristic size for opaque struct */
    struct curl_pushheaders *dummy_h_ptr = malloc(dummy_struct_size);
    if (dummy_h_ptr) {
        /* Zero out the memory to avoid random garbage, though the struct is opaque */
        memset(dummy_h_ptr, 0, dummy_struct_size);
        
        /* Test with index 0 and various indices */
        char *h3 = curl_pushheader_bynum(dummy_h_ptr, 0);
        (void)h3;
        
        /* Test with a large index */
        char *h4 = curl_pushheader_bynum(dummy_h_ptr, 1000);
        (void)h4;

        /* Test with various header names */
        if (Size > 0) {
            char *safe_name = create_safe_string(Data, Size);
            if (safe_name) {
                char *h5 = curl_pushheader_byname(dummy_h_ptr, safe_name);
                (void)h5;
                free(safe_name);
            }
        }
        
        /* Test with empty name */
        char *h6 = curl_pushheader_byname(dummy_h_ptr, "");
        (void)h6;

        /* Test with malformed name */
        char *h7 = curl_pushheader_byname(dummy_h_ptr, ":");
        (void)h7;
        
        free(dummy_h_ptr);
    }

    /* 2. Cleanup */
    /* curl_version returns static memory, no free needed.
     * curl_getenv, curl_maprintf results are freed above.
     * curl_pushheader_* return pointers to internal memory or NULL, do not free.
     */

    return 0;
}