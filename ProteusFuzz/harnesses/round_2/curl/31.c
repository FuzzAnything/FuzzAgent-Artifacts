// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void consume_fuzz_data(const uint8_t *Data, size_t Size, char **out_str, int *out_len) {
    if (Size == 0) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }
    
    char *safe_str = malloc(Size + 1);
    if (!safe_str) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }
    
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';
    
    for (size_t i = 0; i < Size; i++) {
        if (safe_str[i] == '\0') {
            safe_str[i] = ' ';
        }
    }
    
    *out_str = safe_str;
    *out_len = (int)Size;
}

static int test_mvsprintf_variadic(const char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char *fuzz_str = NULL;
    int fuzz_len = 0;
    consume_fuzz_data(Data, Size, &fuzz_str, &fuzz_len);

    if (!fuzz_str) {
        return 0;
    }

    // Test curl_msnprintf
    char buffer1[256];
    int ret1 = curl_msnprintf(buffer1, sizeof(buffer1), "%s", fuzz_str);
    (void)ret1;

    int ret2 = curl_msnprintf(buffer1, sizeof(buffer1), "%d", (int)Data[0]);
    (void)ret2;
    
    int ret3 = curl_msnprintf(buffer1, sizeof(buffer1), "%x", (unsigned int)Data[0]);
    (void)ret3;

    // Test curl_maprintf
    char *mapped_str = curl_maprintf("%s", fuzz_str);
    if (mapped_str) {
        curl_free(mapped_str);
    }
    
    mapped_str = curl_maprintf("%d", (int)Data[0]);
    if (mapped_str) {
        curl_free(mapped_str);
    }

    // Test curl_msprintf
    char buffer2[256];
    int ret4 = curl_msprintf(buffer2, "%s", fuzz_str);
    (void)ret4;

    // Test curl_unescape
    char *unescaped = curl_unescape(fuzz_str, fuzz_len);
    if (unescaped) {
        curl_free(unescaped);
    }
    
    unescaped = curl_unescape(fuzz_str, -1);
    if (unescaped) {
        curl_free(unescaped);
    }

    // Test curl_mvsprintf via variadic helper function
    int ret5 = test_mvsprintf_variadic("%s", fuzz_str);
    (void)ret5;

    // Test curl_easy_escape
    CURL *handle = curl_easy_init();
    if (handle) {
        char *escaped = curl_easy_escape(handle, fuzz_str, fuzz_len);
        if (escaped) {
            curl_free(escaped);
        }
        
        escaped = curl_easy_escape(handle, fuzz_str, 0);
        if (escaped) {
            curl_free(escaped);
        }
        
        curl_easy_cleanup(handle);
    }

    free(fuzz_str);
    return 0;
}