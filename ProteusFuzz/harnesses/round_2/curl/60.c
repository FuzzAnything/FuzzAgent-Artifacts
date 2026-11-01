// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "curl.h"
#include "mprintf.h"

/* Helper function to wrap curl_mvsnprintf with a va_list */
static int helper_mvsnprintf(char *buffer, size_t maxlength, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = curl_mvsnprintf(buffer, maxlength, fmt, args);
    va_end(args);
    return ret;
}

/* Helper function to wrap curl_mvsprintf with a va_list */
static int helper_mvsprintf(char *buffer, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = curl_mvsprintf(buffer, fmt, args);
    va_end(args);
    return ret;
}

static void parse_input(const uint8_t *Data, size_t Size, 
                        char **out_buf, size_t *out_buf_size,
                        char **out_format, size_t *out_format_size,
                        char **out_s1, size_t *out_s1_size,
                        char **out_s2, size_t *out_s2_size,
                        size_t *out_n,
                        int *out_len) {
    *out_buf = NULL;
    *out_buf_size = 0;
    *out_format = NULL;
    *out_format_size = 0;
    *out_s1 = NULL;
    *out_s1_size = 0;
    *out_s2 = NULL;
    *out_s2_size = 0;
    *out_n = 0;
    *out_len = 0;

    if (Size < 1) return;

    size_t offset = 0;
    
    // Determine buffer size (max 1024)
    size_t buf_size = (Size > 1024) ? 1024 : Size;
    if (offset + buf_size <= Size) {
        *out_buf = malloc(buf_size + 1);
        if (*out_buf) {
            memcpy(*out_buf, Data + offset, buf_size);
            (*out_buf)[buf_size] = '\0';
            *out_buf_size = buf_size;
            offset += buf_size;
        }
    }

    // Determine format string size (max 256)
    size_t fmt_size = (Size - offset > 256) ? 256 : (Size - offset);
    if (fmt_size > 0) {
        *out_format = malloc(fmt_size + 1);
        if (*out_format) {
            memcpy(*out_format, Data + offset, fmt_size);
            (*out_format)[fmt_size] = '\0';
            *out_format_size = fmt_size;
            offset += fmt_size;
        }
    }

    // Determine s1 size (max 128)
    size_t s1_size = (Size - offset > 128) ? 128 : (Size - offset);
    if (s1_size > 0) {
        *out_s1 = malloc(s1_size + 1);
        if (*out_s1) {
            memcpy(*out_s1, Data + offset, s1_size);
            (*out_s1)[s1_size] = '\0';
            *out_s1_size = s1_size;
            offset += s1_size;
        }
    }

    // Determine s2 size (max 128)
    size_t s2_size = (Size - offset > 128) ? 128 : (Size - offset);
    if (s2_size > 0) {
        *out_s2 = malloc(s2_size + 1);
        if (*out_s2) {
            memcpy(*out_s2, Data + offset, s2_size);
            (*out_s2)[s2_size] = '\0';
            *out_s2_size = s2_size;
            offset += s2_size;
        }
    }

    // Extract n and length if available
    if (offset < Size) {
        *out_n = (size_t)Data[offset];
        offset++;
    }
    if (offset < Size) {
        *out_len = (int)Data[offset];
        offset++;
    }
}

static void cleanup(char *buf, char *format, char *s1, char *s2, char *escaped) {
    if (buf) free(buf);
    if (format) free(format);
    if (s1) free(s1);
    if (s2) free(s2);
    if (escaped) curl_free(escaped);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    char *buf = NULL;
    size_t buf_size = 0;
    char *format = NULL;
    size_t format_size = 0;
    char *s1 = NULL;
    size_t s1_size = 0;
    char *s2 = NULL;
    size_t s2_size = 0;
    size_t n = 0;
    int len = 0;

    parse_input(Data, Size, &buf, &buf_size, &format, &format_size, 
                &s1, &s1_size, &s2, &s2_size, &n, &len);

    // Ensure we have at least a buffer for formatting functions
    if (!buf) {
        buf = malloc(256);
        if (buf) {
            buf_size = 256;
            buf[0] = '\0';
        }
    }

    // 1. Test curl_msnprintf
    if (buf && format) {
        int ret = curl_msnprintf(buf, buf_size, format, 123, 456, "test");
        (void)ret;
    }

    // 2. Test curl_strnequal
    if (s1 && s2) {
        size_t cmp_n = (n > 0 && n < 256) ? n : 10;
        int ret = curl_strnequal(s1, s2, cmp_n);
        (void)ret;
    }
    // Test with NULLs
    int ret_null = curl_strnequal(NULL, NULL, 5);
    (void)ret_null;
    int ret_one_null = curl_strnequal(s1, NULL, 5);
    (void)ret_one_null;

    // 3. Test curl_maprintf
    char *mapped = NULL;
    if (format) {
        mapped = curl_maprintf(format, 123, 456);
        if (mapped) {
            size_t mlen = strlen(mapped);
            (void)mlen;
            curl_free(mapped);
        }
    }

    // 4 & 5. Test curl_mvsprintf and curl_mvsnprintf using helpers
    if (buf && format) {
        int ret_snprintf = helper_mvsnprintf(buf, buf_size, format, 0);
        (void)ret_snprintf;
        
        // Ensure buffer is large enough for mvsprintf to avoid potential issues
        if (buf_size < 512) {
            char *temp_buf = realloc(buf, 512);
            if (temp_buf) {
                buf = temp_buf;
                buf_size = 512;
            }
        }
        if (buf) {
            int ret_sprintf = helper_mvsprintf(buf, format, 0);
            (void)ret_sprintf;
        }
    }

    // 6. Test curl_escape
    char *escaped = NULL;
    if (s1) {
        int esc_len = (len > 0 && len < 1000) ? len : (int)s1_size;
        escaped = curl_escape(s1, esc_len);
        if (escaped) {
            size_t elen = strlen(escaped);
            (void)elen;
            curl_free(escaped);
        }
    }

    cleanup(buf, format, s1, s2, escaped);

    return 0;
}