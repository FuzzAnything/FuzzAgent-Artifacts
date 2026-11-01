// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>
#include <stdarg.h>

// Helper function to allow va_start usage in a variadic context
static void test_mvfprintf(FILE *fp, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    // Fuzz curl_mvfprintf with the provided format and args
    curl_mvfprintf(fp, format, args);
    
    va_end(args);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Ensure the input data is treated as a string by null-terminating a copy
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Fuzz curl_easy_escape
    // Test with length 0 (null-terminated)
    char *escaped1 = curl_easy_escape(handle, input_str, 0);
    if (escaped1) {
        curl_free(escaped1);
    }
    // Test with explicit length (Size)
    char *escaped2 = curl_easy_escape(handle, input_str, (int)Size);
    if (escaped2) {
        curl_free(escaped2);
    }
    // Test with length 1 (minimal valid non-zero length)
    char *escaped3 = curl_easy_escape(handle, input_str, 1);
    if (escaped3) {
        curl_free(escaped3);
    }

    // Fuzz curl_escape (deprecated wrapper)
    char *escaped4 = curl_escape(input_str, 0);
    if (escaped4) {
        curl_free(escaped4);
    }
    char *escaped5 = curl_escape(input_str, (int)Size);
    if (escaped5) {
        curl_free(escaped5);
    }

    // Fuzz curl_mfprintf
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        // Case 1: Input as format string, no args
        curl_mfprintf(fp, input_str);
        
        // Case 2: Input as format string, with one int arg
        int dummy_int = 0;
        if (Size > 0) {
            dummy_int = Data[0];
        }
        curl_mfprintf(fp, input_str, dummy_int);
        
        // Case 3: Fixed format, input as string arg
        curl_mfprintf(fp, "Data: %s", input_str);
        
        fclose(fp);
    }

    // Fuzz curl_mvfprintf via helper
    fp = fopen("./dummy_file", "w");
    if (fp) {
        // Case 1: Input as format, no extra args
        test_mvfprintf(fp, input_str);
        
        // Case 2: Input as format, with one int arg
        int dummy_int2 = 0;
        if (Size > 1) {
            dummy_int2 = Data[1];
        }
        test_mvfprintf(fp, input_str, dummy_int2);
        
        // Case 3: Fixed format, input as string arg
        test_mvfprintf(fp, "Data: %s", input_str);

        fclose(fp);
    }

    // Cleanup
    curl_easy_cleanup(handle);
    free(input_str);
    
    return 0;
}