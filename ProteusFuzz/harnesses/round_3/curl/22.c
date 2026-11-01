// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl MIME and slist functions.
 * Targets: curl_mime_filename, curl_slist_append, curl_slist_free_all, curl_mime_headers.
 * Note: Wcurl_easy_setopt_err_* are compile-time macros, not runtime functions.
 */

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl if not already done (thread-safe in most versions) */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle and a MIME structure */
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        curl_global_cleanup();
        return 0;
    }

    curl_mime *mime = curl_mime_init(curl_handle);
    if (!mime) {
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to generate filenames and headers */
    char *filename = NULL;

    /* Ensure we have a null-terminated string for filename */
    if (Size > 0) {
        filename = (char *)malloc(Size + 1);
        if (filename) {
            memcpy(filename, Data, Size);
            filename[Size] = '\0';
        }
    }

    /* Test curl_mime_filename with valid part and filename */
    if (filename) {
        curl_mime_filename(part, filename);
        /* Also test with NULL filename to explore error paths */
        curl_mime_filename(part, NULL);
        free(filename);
    }

    /* Test curl_mime_filename with NULL part (should return CURLE_BAD_FUNCTION_ARGUMENT) */
    curl_mime_filename(NULL, "test.txt");

    /* Build a curl_slist for headers */
    struct curl_slist *headers = NULL;

    /* Split input data into chunks to create multiple headers */
    size_t chunk_size = Size / 4;
    if (chunk_size == 0) chunk_size = 1;

    for (size_t i = 0; i < 4 && i * chunk_size < Size; i++) {
        size_t len = (i + 1) * chunk_size < Size ? chunk_size : Size - i * chunk_size;
        char *chunk = (char *)malloc(len + 1);
        if (!chunk) continue;
        memcpy(chunk, Data + i * chunk_size, len);
        chunk[len] = '\0';

        /* Append to slist */
        headers = curl_slist_append(headers, chunk);
        free(chunk);
    }

    /* Test curl_mime_headers with valid part and headers */
    if (headers) {
        /* Test with take_ownership = 1 (libcurl frees the list) */
        curl_mime_headers(part, headers, 1);

        /* Re-create list for next test */
        headers = NULL;
        for (size_t i = 0; i < 4 && i * chunk_size < Size; i++) {
            size_t len = (i + 1) * chunk_size < Size ? chunk_size : Size - i * chunk_size;
            char *chunk = (char *)malloc(len + 1);
            if (!chunk) continue;
            memcpy(chunk, Data + i * chunk_size, len);
            chunk[len] = '\0';
            headers = curl_slist_append(headers, chunk);
            free(chunk);
        }

        /* Test with take_ownership = 0 (caller must free) */
        curl_mime_headers(part, headers, 0);
        curl_slist_free_all(headers);
    }

    /* Test curl_mime_headers with NULL part */
    headers = curl_slist_append(NULL, "Content-Type: text/plain");
    curl_mime_headers(NULL, headers, 0);
    curl_slist_free_all(headers);

    /* Test curl_slist_append with NULL list */
    headers = curl_slist_append(NULL, "Test-Header: Value");
    if (headers) {
        headers = curl_slist_append(headers, "Another-Header: Data");
        /* Test appending empty string */
        headers = curl_slist_append(headers, "");
        /* 
         * Note: curl_slist_append expects a valid string pointer. 
         * Passing NULL causes a crash inside libcurl (curlx_strdup).
         * We skip this invalid test case to prevent sanitizer errors.
         */
        curl_slist_free_all(headers);
    }

    /* Cleanup */
    curl_mime_free(mime);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return 0;
}