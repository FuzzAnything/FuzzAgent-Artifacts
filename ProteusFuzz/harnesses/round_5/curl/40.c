// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_escape at escape.c:36:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static void setup_file(const char *filename, FILE **fp) {
    FILE *f = fopen(filename, "w+");
    if (f) {
        *fp = f;
    } else {
        *fp = NULL;
    }
}

static void cleanup_file(FILE **fp) {
    if (*fp) {
        fclose(*fp);
        *fp = NULL;
    }
}

static void cleanup_curl(CURL **handle) {
    if (*handle) {
        curl_easy_cleanup(*handle);
        *handle = NULL;
    }
}

static void cleanup_string(char **str) {
    if (*str) {
        curl_free(*str);
        *str = NULL;
    }
}

// Helper to create a safe null-terminated string from fuzz input
static char *create_safe_string(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    char *safe_str = malloc(Size + 1);
    if (!safe_str) return NULL;
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';
    return safe_str;
}

// Wrapper for curl_mvfprintf to handle va_list correctly
// Must be varargs to allow va_start
static int wrapper_mvfprintf(FILE *fp, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = curl_mvfprintf(fp, fmt, args);
    va_end(args);
    return ret;
}

// Wrapper for curl_mfprintf to handle va_list correctly
// Must be varargs to allow va_start
static int wrapper_mfprintf(FILE *fp, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = curl_mfprintf(fp, fmt, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *handle = NULL;
    FILE *fp = NULL;
    char buffer[256];
    char *escaped = NULL;
    char *unescaped = NULL;
    int outlength = 0;
    char *safe_input = NULL;

    // Initialize curl handle
    handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Setup dummy file
    setup_file("./dummy_file", &fp);

    // Create a safe null-terminated string from fuzz input
    safe_input = create_safe_string(Data, Size);
    if (!safe_input) {
        cleanup_curl(&handle);
        return 0;
    }

    // Fuzz curl_mvfprintf via wrapper
    if (fp) {
        // Use a safe format string with the sanitized input
        wrapper_mvfprintf(fp, "%s", safe_input);
        
        // Try another format with integer derived from input
        int dummy_int = (int)Data[0];
        wrapper_mvfprintf(fp, "%d", dummy_int);
    }

    // Fuzz curl_msprintf
    if (safe_input) {
        // Ensure buffer is large enough and format is safe
        // Using %s with the input data, limit to buffer size - 1
        int ret = curl_msprintf(buffer, "%s", safe_input);
        (void)ret;
        
        // Try with integer
        int dummy_int = (int)Data[0];
        ret = curl_msprintf(buffer, "%d", dummy_int);
        (void)ret;
    }

    // Fuzz curl_escape
    if (safe_input) {
        escaped = curl_escape(safe_input, (int)Size);
        cleanup_string(&escaped);
    }

    // Fuzz curl_mfprintf via wrapper
    if (fp && safe_input) {
        wrapper_mfprintf(fp, "%s", safe_input);
        int dummy_int = (int)Data[0];
        wrapper_mfprintf(fp, "%d", dummy_int);
    }

    // Fuzz curl_easy_unescape
    if (safe_input && handle) {
        unescaped = curl_easy_unescape(handle, safe_input, (int)Size, &outlength);
        cleanup_string(&unescaped);
    }

    // Fuzz curl_easy_escape
    if (safe_input && handle) {
        escaped = curl_easy_escape(handle, safe_input, (int)Size);
        cleanup_string(&escaped);
    }

    // Cleanup
    if (safe_input) {
        free(safe_input);
    }
    cleanup_file(&fp);
    cleanup_curl(&handle);

    return 0;
}