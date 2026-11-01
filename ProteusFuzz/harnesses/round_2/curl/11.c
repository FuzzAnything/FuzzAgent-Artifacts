// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strequal at strequal.c:76:5 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/header.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_header(struct curl_header *h) {
    (void)h;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    /* Set a simple URL to allow header functions to have context */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);

    /* Perform a request to populate headers */
    curl_easy_perform(easy);

    /* Fuzz curl_msprintf */
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (Size >= 2) {
        char safe_fmt[256];
        size_t copy_len = Size < 255 ? Size : 255;
        memcpy(safe_fmt, Data, copy_len);
        safe_fmt[copy_len] = '\0';
        /* Only use simple format specifiers to avoid crashes */
        if (strstr(safe_fmt, "%s") || strstr(safe_fmt, "%d") || strstr(safe_fmt, "%x")) {
            curl_msprintf(buffer, safe_fmt, "test", 42, 0xdead);
        } else {
            curl_msprintf(buffer, "fixed");
        }
    }

    /* Fuzz curl_easy_header */
    struct curl_header *hout = NULL;
    if (Size >= 4) {
        const char *name = (const char *)Data;
        size_t index = (size_t)Data[Size - 1];
        unsigned int origin = (unsigned int)Data[Size - 2];
        int request = (int)Data[Size - 3];
        curl_easy_header(easy, name, index, origin, request, &hout);
        cleanup_header(hout);
        hout = NULL;
    }

    /* Fuzz curl_easy_unescape */
    char *unescaped = NULL;
    int outlen = 0;
    if (Size >= 3) {
        const char *str = (const char *)Data;
        int len = (int)Data[Size - 1];
        if (len > (int)Size - 2) len = (int)Size - 2;
        unescaped = curl_easy_unescape(easy, str, len, &outlen);
        if (unescaped) {
            free(unescaped);
            unescaped = NULL;
        }
    }

    /* Fuzz curl_easy_nextheader */
    struct curl_header *prev = NULL;
    struct curl_header *next = NULL;
    if (Size >= 3) {
        unsigned int origin = (unsigned int)Data[Size - 1];
        int request = (int)Data[Size - 2];
        next = curl_easy_nextheader(easy, origin, request, prev);
        cleanup_header(next);
        if (next) {
            prev = next;
            next = curl_easy_nextheader(easy, origin, request, prev);
            cleanup_header(next);
        }
    }

    /* Fuzz curl_strequal */
    /* Ensure both strings are null-terminated to prevent heap-buffer-overflow */
    if (Size >= 2) {
        char s1_buf[256];
        char s2_buf[256];
        
        /* Copy first part */
        size_t len1 = Size < 255 ? Size : 255;
        memcpy(s1_buf, Data, len1);
        s1_buf[len1] = '\0';
        
        /* Copy second part starting from index 1 */
        size_t len2 = Size - 1;
        if (len2 > 255) len2 = 255;
        memcpy(s2_buf, Data + 1, len2);
        s2_buf[len2] = '\0';
        
        int res = curl_strequal(s1_buf, s2_buf);
        (void)res;
    }

    /* Fuzz curl_mprintf */
    if (Size >= 2) {
        char safe_fmt[256];
        size_t copy_len = Size < 255 ? Size : 255;
        memcpy(safe_fmt, Data, copy_len);
        safe_fmt[copy_len] = '\0';
        if (strstr(safe_fmt, "%s") || strstr(safe_fmt, "%d")) {
            curl_mprintf(safe_fmt, "test", 123);
        } else {
            curl_mprintf("fixed output");
        }
    }

    /* Cleanup */
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    return 0;
}