// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void test_mvfprintf(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvfprintf(f, fmt, args);
    va_end(args);
}

static void test_mvprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvprintf(fmt, args);
    va_end(args);
}

static void test_mvsprintf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvsprintf(buf, fmt, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null-termination for string operations if needed */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* Test curl_maprintf */
    /* Ensure we have enough data to access indices 0 and 1 safely */
    if (Size >= 2) {
        char *maprintf_result = curl_maprintf("%s %d %c", input_str, (int)Data[0], (char)Data[1]);
        if (maprintf_result) {
            curl_free(maprintf_result);
        }
    } else if (Size == 1) {
        char *maprintf_result = curl_maprintf("%s %d", input_str, (int)Data[0]);
        if (maprintf_result) {
            curl_free(maprintf_result);
        }
    } else {
        /* Size is 0, handled at start, but for completeness */
        char *maprintf_result = curl_maprintf("%s", input_str);
        if (maprintf_result) {
            curl_free(maprintf_result);
        }
    }

    /* Test curl_msnprintf */
    char buffer[256];
    if (Size >= 3) {
        curl_msnprintf(buffer, sizeof(buffer), "%s %d", input_str, (int)Data[2]);
    } else {
        curl_msnprintf(buffer, sizeof(buffer), "%s", input_str);
    }

    /* Test curl_unescape with length -1 (null terminated) */
    char *unescape_result1 = curl_unescape(input_str, -1);
    if (unescape_result1) {
        curl_free(unescape_result1);
    }

    /* Test curl_unescape with specific length (ensure non-zero length) */
    int unescape_len = (Size > 0) ? (int)Size : 1;
    char *unescape_result2 = curl_unescape(input_str, unescape_len);
    if (unescape_result2) {
        curl_free(unescape_result2);
    }

    /* Test curl_mvfprintf */
    write_dummy_file(Data, Size);
    FILE *f = fopen("./dummy_file", "r");
    if (f) {
        test_mvfprintf(f, "%s", input_str);
        fclose(f);
    }

    /* Test curl_mvprintf */
    test_mvprintf("%s", input_str);

    /* Test curl_mvsprintf */
    /* Allocate dynamic buffer to avoid overflow as per warning */
    size_t mvsprintf_buf_size = Size * 2 + 100;
    char *mvsprintf_buf = (char *)malloc(mvsprintf_buf_size);
    if (mvsprintf_buf) {
        test_mvsprintf(mvsprintf_buf, "%s", input_str);
        free(mvsprintf_buf);
    }

    free(input_str);
    return 0;
}