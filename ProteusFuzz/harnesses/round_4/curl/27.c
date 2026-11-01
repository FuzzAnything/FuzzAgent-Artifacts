// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Include curl headers. 
   Note: curl/curl.h defines CURL_EXTERN, CURLoption, and other necessary types.
   We include it first to ensure types are defined before options.h and mprintf.h. */
#include "curl/curl.h"
#include "curl/options.h"
#include "curl/mprintf.h"

/* Helper function to safely create a string from input data */
static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

/* Helper function to test curl_mvsprintf safely using a variadic wrapper */
static int test_mvsprintf_variadic(const char *fmt, ...) {
    char buf[256] = {0};
    va_list args;
    va_start(args, fmt);
    int ret = curl_mvsprintf(buf, fmt, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Split input into segments for different fuzzing targets */
    size_t offset = 0;
    size_t seg1_len = 0, seg2_len = 0, seg3_len = 0, seg4_len = 0;
    
    /* Determine segment lengths based on available size */
    if (Size > 0) seg1_len = (Size > 1) ? Size / 4 : 1;
    if (Size > seg1_len) seg2_len = (Size - seg1_len > 1) ? (Size - seg1_len) / 3 : 1;
    if (Size > seg1_len + seg2_len) seg3_len = (Size - seg1_len - seg2_len > 1) ? (Size - seg1_len - seg2_len) / 2 : 1;
    seg4_len = Size - seg1_len - seg2_len - seg3_len;

    char *str1 = NULL, *str2 = NULL, *str3 = NULL, *str4 = NULL;

    str1 = create_safe_string(Data + offset, seg1_len);
    offset += seg1_len;
    str2 = create_safe_string(Data + offset, seg2_len);
    offset += seg2_len;
    str3 = create_safe_string(Data + offset, seg3_len);
    offset += seg3_len;
    str4 = create_safe_string(Data + offset, seg4_len);

    /* 1. Test curl_easy_option_next */
    const struct curl_easyoption *prev = NULL;
    const struct curl_easyoption *next = NULL;
    
    /* Start iteration */
    next = curl_easy_option_next(prev);
    while (next != NULL) {
        prev = next;
        next = curl_easy_option_next(prev);
    }

    /* 2. Test curl_maprintf */
    char *mapped_str = NULL;
    if (str2 && seg2_len > 0) {
        /* Pass the string as format, with dummy arguments to avoid immediate crash if format is invalid */
        mapped_str = curl_maprintf(str2, 123, 456, "test");
        if (mapped_str) {
            curl_free(mapped_str);
            mapped_str = NULL;
        }
    }

    /* 3. Test curl_easy_option_by_name */
    const struct curl_easyoption *opt_by_name = NULL;
    if (str1 && seg1_len > 0) {
        opt_by_name = curl_easy_option_by_name(str1);
        /* No cleanup needed for returned const pointer */
    }

    /* 4. Test curl_msnprintf */
    char buffer[256] = {0};
    int ret_snprintf = 0;
    if (str3 && seg3_len > 0) {
        ret_snprintf = curl_msnprintf(buffer, sizeof(buffer), str3, 789, "fuzz");
        /* ret_snprintf is int, check for -1 on error */
    }

    /* 5. Test curl_easy_option_by_id */
    const struct curl_easyoption *opt_by_id = NULL;
    /* Generate an ID from input data if possible, or use a range */
    CURLoption test_id = (CURLoption)0;
    if (Size > 0) {
        test_id = (CURLoption)(Data[0] & 0xFF);
        /* Try a few variations */
        opt_by_id = curl_easy_option_by_id(test_id);
        opt_by_id = curl_easy_option_by_id((CURLoption)(Data[0] | 0x100));
    }

    /* 6. Test curl_mvsprintf via helper */
    int ret_vsprintf = 0;
    if (str4 && seg4_len > 0) {
        /* Use the variadic wrapper to correctly handle va_list */
        ret_vsprintf = test_mvsprintf_variadic(str4, 111, "dummy");
    }
    
    /* Cleanup */
    if (str1) free(str1);
    if (str2) free(str2);
    if (str3) free(str3);
    if (str4) free(str4);
    if (mapped_str) curl_free(mapped_str);

    return 0;
}