// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static void setup_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "dummy data for testing\n");
        fclose(f);
    }
}

/* Helper functions defined outside LLVMFuzzerTestOneInput to allow va_start */
static int helper_fprintf(FILE *f, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = curl_mvfprintf(f, fmt, ap);
    va_end(ap);
    return res;
}

static int helper_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = curl_mvprintf(fmt, ap);
    va_end(ap);
    return res;
}

static int helper_sprintf(char *buf, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = curl_mvsprintf(buf, fmt, ap);
    va_end(ap);
    return res;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Ensure null-termination for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* Initialize CURL handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        free(input_str);
        return 0;
    }

    /* Setup dummy file for curl_mvfprintf */
    setup_dummy_file("./dummy_file");
    FILE *fd = fopen("./dummy_file", "w");

    /* Test 1: curl_maprintf */
    char *maprintf_result = NULL;
    maprintf_result = curl_maprintf("%s", input_str);
    if (maprintf_result) {
        curl_free(maprintf_result);
    }

    /* Test 2: curl_easy_escape */
    char *escaped = curl_easy_escape(handle, input_str, 0);
    if (escaped) {
        curl_free(escaped);
    }

    /* Test 3: curl_msnprintf */
    char buffer[256];
    int ret_snprintf = curl_msnprintf(buffer, sizeof(buffer), "%s", input_str);
    (void)ret_snprintf;

    /* Test 4: curl_mvfprintf */
    if (fd) {
        int ret_fprintf = helper_fprintf(fd, "%s", input_str);
        (void)ret_fprintf;
        fclose(fd);
    }

    /* Test 5: curl_mvprintf */
    int ret_printf = helper_printf("%s", input_str);
    (void)ret_printf;

    /* Test 6: curl_mvsprintf */
    /* Allocate a large enough buffer to prevent overflow */
    char *vsprintf_buf = (char *)malloc(Size + 128);
    if (vsprintf_buf) {
        int ret_sprintf = helper_sprintf(vsprintf_buf, "%s", input_str);
        (void)ret_sprintf;
        free(vsprintf_buf);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    free(input_str);

    return 0;
}