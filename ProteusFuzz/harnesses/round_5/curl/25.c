// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Required curl headers */
#include "curl.h"
#include "mprintf.h"
#include "header.h"
#include "typecheck-gcc.h"

/* Static helper to write dummy data to a file if needed */
static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare some buffers for testing */
    char buffer[4096];
    char *unescaped = NULL;
    int outlength = 0;
    struct curl_header *hout = NULL;
    struct curl_header *prev = NULL;

    /* Use the fuzzer data as input strings */
    const char *input_str = (const char *)Data;
    size_t input_len = Size;

    /* Ensure safe string handling */
    char safe_str[256];
    size_t copy_len = (input_len < 255) ? input_len : 255;
    memcpy(safe_str, input_str, copy_len);
    safe_str[copy_len] = '\0';

    /* 1. Test curl_mprintf with various formats derived from input */
    /* We use a simple format string to avoid format string vulnerabilities */
    curl_mprintf("%s\n", safe_str);
    
    /* Test curl_msprintf */
    int ret = curl_msprintf(buffer, "%s", safe_str);
    (void)ret; /* Suppress unused variable warning */

    /* 2. Test curl_easy_unescape */
    /* We need a valid handle for this */
    unescaped = curl_easy_unescape(easy, input_str, (int)input_len, &outlength);
    if (unescaped) {
        /* Do something with unescaped to avoid unused warning */
        (void)strlen(unescaped);
        curl_free(unescaped);
        unescaped = NULL;
    }

    /* 3. Test curl_easy_header and curl_easy_nextheader */
    /* These functions typically require a completed request with headers.
       Since we haven't performed a real request, these will likely return errors
       or NULL, which is expected behavior for fuzzing edge cases. */
    
    /* Try to get a header by name */
    CURLHcode code = curl_easy_header(easy, safe_str ? safe_str : "Host", 0, 
                                      CURLH_HEADER, 0, &hout);
    (void)code; /* Suppress unused variable warning */
    if (hout) {
        /* Header found, though unlikely without a real request */
        (void)hout->name;
        (void)hout->value;
    }

    /* Try to iterate headers */
    struct curl_header *next = curl_easy_nextheader(easy, CURLH_HEADER, 0, prev);
    (void)next; /* Suppress unused variable warning */

    /* 4. Test curl_easy_setopt with various options using fuzzer data */
    /* We use the fuzzer data as string arguments where appropriate */
    char *data_ptr = (char *)Data;
    
    /* Set URL if data looks like a URL (starts with http) */
    if (Size >= 4 && memcmp(Data, "http", 4) == 0) {
        /* Ensure null termination */
        char url_buf[1024];
        size_t url_len = (Size < 1023) ? Size : 1023;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(easy, CURLOPT_URL, url_buf);
    }

    /* Set POST fields if data is binary/string */
    /* CRITICAL FIX: We must allocate memory that persists beyond this scope 
       or use a static buffer, because curl_easy_perform may access it later.
       Using a static buffer ensures the data remains valid during the perform call. */
    static char post_buf_static[4096];
    if (Size > 0) {
        size_t post_len = (Size < sizeof(post_buf_static) - 1) ? Size : sizeof(post_buf_static) - 1;
        memcpy(post_buf_static, Data, post_len);
        post_buf_static[post_len] = '\0';
        curl_easy_setopt(easy, CURLOPT_POSTFIELDS, post_buf_static);
    }

    /* Set a custom header */
    if (Size > 5) {
        char header_buf[1024];
        size_t h_len = (Size < 1023) ? Size : 1023;
        memcpy(header_buf, Data, h_len);
        header_buf[h_len] = '\0';
        
        /* Construct a valid header string "Key: Value" */
        char full_header[1024];
        snprintf(full_header, sizeof(full_header), "X-Fuzz: %s", header_buf);
        
        struct curl_slist *headers = curl_slist_append(NULL, full_header);
        if (headers) {
            curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
            curl_slist_free_all(headers);
        }
    }

    /* 5. Perform a dummy request to potentially populate headers for later tests */
    /* We use a timeout to prevent hanging */
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 100);
    curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT_MS, 100);
    curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy, CURLOPT_FAILONERROR, 0L);
    
    /* Perform the request (will likely fail due to invalid URL, but that's okay for fuzzing) */
    CURLcode res = curl_easy_perform(easy);
    (void)res;

    /* Try header functions again after a (failed) request */
    hout = NULL;
    code = curl_easy_header(easy, "Content-Type", 0, CURLH_HEADER, 0, &hout);
    (void)code;
    if (hout) {
        (void)hout->name;
    }

    next = curl_easy_nextheader(easy, CURLH_HEADER, 0, NULL);
    (void)next;

    /* Cleanup */
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    return 0;
}