// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null-termination for string operations
    char *input_str = NULL;
    size_t max_len = 1024;
    if (Size < max_len) {
        input_str = malloc(Size + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    } else {
        input_str = malloc(max_len + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, max_len);
        input_str[max_len] = '\0';
    }

    // 1. Fuzz curl_getenv
    // We treat the input as a potential environment variable name
    char *env_result = curl_getenv(input_str);
    if (env_result) {
        // Use the result briefly to ensure it's valid
        (void)strlen(env_result);
        curl_free(env_result);
    }

    // 2. Fuzz curl_version
    // This function takes no arguments and returns a static string.
    // We call it to ensure the library is initialized and the string is accessible.
    const char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    // 3. Fuzz curl_share_strerror
    // We generate various error codes from the input data to test edge cases
    for (size_t i = 0; i < Size; i++) {
        CURLSHcode code = (CURLSHcode)Data[i];
        const char *err_str = curl_share_strerror(code);
        if (err_str) {
            (void)strlen(err_str);
        }
    }
    // Test with a larger value derived from input
    if (Size >= 2) {
        uint16_t val16 = (Data[0] << 8) | Data[1];
        const char *err_str16 = curl_share_strerror((CURLSHcode)val16);
        if (err_str16) {
            (void)strlen(err_str16);
        }
    }

    // 5. Fuzz curl_maprintf
    // We use the input as a format string. This is risky but good for fuzzing.
    // We limit the format string length to prevent excessive allocation.
    if (Size < 100) {
        char *fmt_result = curl_maprintf(input_str);
        if (fmt_result) {
            (void)strlen(fmt_result);
            curl_free(fmt_result);
        }
    }
    // Test with a fixed format and input data as arguments
    if (Size >= 4) {
        int arg1 = (int)Data[0];
        int arg2 = (int)Data[1];
        int arg3 = (int)Data[2];
        char *fmt_result2 = curl_maprintf("Test: %d %d %s", arg1, arg2, input_str);
        if (fmt_result2) {
            (void)strlen(fmt_result2);
            curl_free(fmt_result2);
        }
    }

    // 6. Fuzz curl_msnprintf
    char out_buffer[512];
    if (Size < 100) {
        int ret = curl_msnprintf(out_buffer, sizeof(out_buffer), input_str);
        (void)ret;
    }
    // Test with fixed format and input data
    if (Size >= 2) {
        int arg_val = (int)Data[0];
        int ret2 = curl_msnprintf(out_buffer, sizeof(out_buffer), "Value: %d, Str: %s", arg_val, input_str);
        (void)ret2;
    }

    // Cleanup
    free(input_str);

    // Write dummy file if needed (though not strictly required for these specific APIs, 
    // it satisfies the requirement if any future logic depends on it)
    write_dummy_file(Data, Size);

    return 0;
}