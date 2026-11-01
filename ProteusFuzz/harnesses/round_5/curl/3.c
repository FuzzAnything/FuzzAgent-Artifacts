// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/options.h>

static void safe_free(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

static int call_mvsprintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char buf[512];
    int ret = curl_mvsprintf(buf, fmt, ap);
    va_end(ap);
    return ret;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure input is null-terminated for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test curl_getenv
    // Pass the fuzz input as the environment variable name
    char *env_result = curl_getenv(input_str);
    safe_free(env_result);

    // 2. Test curl_maprintf
    // Use a safe format string and the input as the argument
    char *maprintf_result = curl_maprintf("prefix: %s suffix", input_str);
    safe_free(maprintf_result);

    // Test with a simple integer format
    char *maprintf_result2 = curl_maprintf("test: %d", (int)Data[0]);
    safe_free(maprintf_result2);

    // 3. Test curl_easy_option_next
    const struct curl_easyoption *opt = NULL;
    // Iterate through all options
    while ((opt = curl_easy_option_next(opt)) != NULL) {
        // Just iterate to exercise the function
        (void)opt;
    }

    // 4. Test curl_easy_option_by_name
    // Use the input as the option name
    const struct curl_easyoption *opt_by_name = curl_easy_option_by_name(input_str);
    if (opt_by_name) {
        // Access fields to ensure pointer is valid
        (void)opt_by_name->name;
        (void)opt_by_name->id;
        (void)opt_by_name->type;
        (void)opt_by_name->flags;
    }

    // 5. Test curl_easy_option_by_id
    // Use the first byte of input as an ID (cast to CURLoption)
    CURLoption id = (CURLoption)Data[0];
    const struct curl_easyoption *opt_by_id = curl_easy_option_by_id(id);
    if (opt_by_id) {
        (void)opt_by_id->name;
        (void)opt_by_id->id;
        (void)opt_by_id->type;
        (void)opt_by_id->flags;
    }

    // 6. Test curl_mvsprintf via helper
    (void)call_mvsprintf("test: %s", input_str);
    (void)call_mvsprintf("num: %d", (int)Data[0]);

    free(input_str);
    return 0;
}