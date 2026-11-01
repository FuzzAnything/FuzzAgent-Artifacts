// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Helper to safely parse simple format strings for fuzzing */
static int count_format_args(const char *fmt) {
    int count = 0;
    const char *p = fmt;
    while (*p) {
        if (*p == '%') {
            p++;
            /* Skip flags, width, precision */
            while (*p && (*p == '-' || *p == '+' || *p == ' ' || *p == '#' || *p == '0' ||
                           *p >= '1' && *p <= '9' || *p == '*' || *p == '.')) {
                p++;
            }
            /* Check for length modifiers */
            if (*p == 'h' || *p == 'l' || *p == 'L' || *p == 'j' || *p == 'z' || *p == 't') {
                p++;
                if (*p == 'h' || *p == 'l') p++;
            }
            /* Check for conversion specifier */
            if (*p) {
                count++;
                p++;
            }
        } else {
            p++;
        }
    }
    return count;
}

/* Helper to get type of next format specifier: 1=s, 2=d/i/u, 3=f, 0=none */
static int get_format_type(const char **p) {
    const char *ptr = *p;
    if (*ptr != '%') return 0;
    ptr++;
    /* Skip flags, width, precision */
    while (*ptr && (*ptr == '-' || *ptr == '+' || *ptr == ' ' || *ptr == '#' || *ptr == '0' ||
                   *ptr >= '1' && *ptr <= '9' || *ptr == '*' || *ptr == '.')) {
        ptr++;
    }
    /* Check for length modifiers */
    if (*ptr == 'h' || *ptr == 'l' || *ptr == 'L' || *ptr == 'j' || *ptr == 'z' || *ptr == 't') {
        ptr++;
        if (*ptr == 'h' || *ptr == 'l') ptr++;
    }
    if (*ptr == 's') {
        *p = ptr + 1;
        return 1;
    } else if (*ptr == 'd' || *ptr == 'i' || *ptr == 'u' || *ptr == 'o' || *ptr == 'x' || *ptr == 'X') {
        *p = ptr + 1;
        return 2;
    } else if (*ptr == 'f' || *ptr == 'e' || *ptr == 'E' || *ptr == 'g' || *ptr == 'G' || *ptr == 'a' || *ptr == 'A') {
        *p = ptr + 1;
        return 3;
    }
    *p = ptr;
    return 0;
}

/* Variadic helper for curl_mvaprintf */
static char *helper_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *res = curl_mvaprintf(format, args);
    va_end(args);
    return res;
}

/* Variadic helper for curl_mvsprintf */
static int helper_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Use part of input as format string, ensuring it's null-terminated */
    size_t fmt_len = 0;
    while (fmt_len < Size && Data[fmt_len] != '\0') {
        fmt_len++;
    }
    if (fmt_len == 0) fmt_len = 1;
    if (fmt_len >= Size) fmt_len = Size - 1;

    char *format = (char *)malloc(fmt_len + 1);
    if (!format) return 0;
    memcpy(format, Data, fmt_len);
    format[fmt_len] = '\0';

    /* Test 1: curl_version - no args, static string */
    char *ver = curl_version();
    if (ver) {
        (void)strlen(ver);
    }

    /* Test 2: curl_getenv - try to get an env var based on input */
    size_t env_len = 0;
    while (env_len < Size && Data[Size - 1 - env_len] != '\0') {
        env_len++;
    }
    if (env_len == 0) env_len = 1;
    if (env_len >= Size) env_len = Size - 1;
    
    char *var_name = (char *)malloc(env_len + 1);
    if (!var_name) {
        free(format);
        return 0;
    }
    memcpy(var_name, Data + (Size - env_len), env_len);
    var_name[env_len] = '\0';

    char *env_val = curl_getenv(var_name);
    if (env_val) {
        curl_free(env_val);
    }
    free(var_name);

    /* Test 3: curl_maprintf - format string with variadic args */
    int num_args = count_format_args(format);
    if (num_args > 10) num_args = 10;

    char *result = NULL;
    const char *dummy_str = "fuzz_test";
    int dummy_int = 0x12345678;
    double dummy_double = 3.14159;

    /* Simple strategy: pass arguments based on detected types */
    if (num_args == 0) {
        result = curl_maprintf(format);
    } else if (num_args == 1) {
        const char *p = format;
        int t = get_format_type(&p);
        if (t == 1) result = curl_maprintf(format, dummy_str);
        else if (t == 2) result = curl_maprintf(format, dummy_int);
        else if (t == 3) result = curl_maprintf(format, dummy_double);
    } else if (num_args == 2) {
        const char *p = format;
        int t1 = get_format_type(&p);
        int t2 = get_format_type(&p);
        
        if (t1 == 1 && t2 == 1) result = curl_maprintf(format, dummy_str, dummy_str);
        else if (t1 == 2 && t2 == 2) result = curl_maprintf(format, dummy_int, dummy_int);
        else if (t1 == 1 && t2 == 2) result = curl_maprintf(format, dummy_str, dummy_int);
        else if (t1 == 2 && t2 == 1) result = curl_maprintf(format, dummy_int, dummy_str);
        else if (t1 == 3 && t2 == 3) result = curl_maprintf(format, dummy_double, dummy_double);
        else if (t1 == 1 && t2 == 3) result = curl_maprintf(format, dummy_str, dummy_double);
        else if (t1 == 3 && t2 == 1) result = curl_maprintf(format, dummy_double, dummy_str);
    }

    if (result) {
        curl_free(result);
    }

    /* Test 4: curl_mvaprintf via helper */
    char *mv_result = NULL;
    if (num_args == 0) {
        mv_result = helper_mvaprintf(format);
    } else if (num_args == 1) {
        const char *p = format;
        int t = get_format_type(&p);
        if (t == 1) mv_result = helper_mvaprintf(format, dummy_str);
        else if (t == 2) mv_result = helper_mvaprintf(format, dummy_int);
        else if (t == 3) mv_result = helper_mvaprintf(format, dummy_double);
    } else if (num_args == 2) {
        const char *p = format;
        int t1 = get_format_type(&p);
        int t2 = get_format_type(&p);
        
        if (t1 == 1 && t2 == 1) mv_result = helper_mvaprintf(format, dummy_str, dummy_str);
        else if (t1 == 2 && t2 == 2) mv_result = helper_mvaprintf(format, dummy_int, dummy_int);
        else if (t1 == 1 && t2 == 2) mv_result = helper_mvaprintf(format, dummy_str, dummy_int);
        else if (t1 == 2 && t2 == 1) mv_result = helper_mvaprintf(format, dummy_int, dummy_str);
        else if (t1 == 3 && t2 == 3) mv_result = helper_mvaprintf(format, dummy_double, dummy_double);
        else if (t1 == 1 && t2 == 3) mv_result = helper_mvaprintf(format, dummy_str, dummy_double);
        else if (t1 == 3 && t2 == 1) mv_result = helper_mvaprintf(format, dummy_double, dummy_str);
    }

    if (mv_result) {
        curl_free(mv_result);
    }

    /* Test 5: curl_mvsprintf via helper */
    char buffer[4096];
    int mvs_result = 0;
    if (num_args == 0) {
        mvs_result = helper_mvsprintf(buffer, format);
    } else if (num_args == 1) {
        const char *p = format;
        int t = get_format_type(&p);
        if (t == 1) mvs_result = helper_mvsprintf(buffer, format, dummy_str);
        else if (t == 2) mvs_result = helper_mvsprintf(buffer, format, dummy_int);
        else if (t == 3) mvs_result = helper_mvsprintf(buffer, format, dummy_double);
    } else if (num_args == 2) {
        const char *p = format;
        int t1 = get_format_type(&p);
        int t2 = get_format_type(&p);
        
        if (t1 == 1 && t2 == 1) mvs_result = helper_mvsprintf(buffer, format, dummy_str, dummy_str);
        else if (t1 == 2 && t2 == 2) mvs_result = helper_mvsprintf(buffer, format, dummy_int, dummy_int);
        else if (t1 == 1 && t2 == 2) mvs_result = helper_mvsprintf(buffer, format, dummy_str, dummy_int);
        else if (t1 == 2 && t2 == 1) mvs_result = helper_mvsprintf(buffer, format, dummy_int, dummy_str);
        else if (t1 == 3 && t2 == 3) mvs_result = helper_mvsprintf(buffer, format, dummy_double, dummy_double);
        else if (t1 == 1 && t2 == 3) mvs_result = helper_mvsprintf(buffer, format, dummy_str, dummy_double);
        else if (t1 == 3 && t2 == 1) mvs_result = helper_mvsprintf(buffer, format, dummy_double, dummy_str);
    }
    
    (void)mvs_result; /* Suppress unused warning */

    free(format);
    return 0;
}