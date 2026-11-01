// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
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
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Helper function to create a null-terminated string from fuzzer data */
static char *create_safe_string(const uint8_t *data, size_t len) {
    if (len == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(len + 1);
    if (!s) return NULL;
    memcpy(s, data, len);
    s[len] = '\0';
    return s;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLcode rc = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (rc != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a MIME structure */
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Create a MIME part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /*
     * Fuzzing logic:
     * We split the input data into segments to drive different functions.
     * Segment 0: Headers string for curl_slist_append
     * Segment 1: Encoding name for curl_mime_encoder
     * Segment 2: Name string for curl_mime_name
     */

    size_t split1 = 0;
    size_t split2 = 0;

    if (Size > 2) {
        split1 = Data[0] % (Size - 1);
        split2 = (Data[1] % (Size - split1 - 1)) + split1;
    } else if (Size > 1) {
        split1 = Data[0] % Size;
        split2 = Size;
    } else {
        split1 = 0;
        split2 = 0;
    }

    /*
     * 1. Test curl_slist_append and curl_mime_headers
     * Build a list of headers from the first segment.
     */
    struct curl_slist *headers = NULL;
    const uint8_t *hdr_data = Data;
    size_t hdr_len = split1;

    /* Ensure we have at least some data for headers if split1 > 0 */
    if (hdr_len > 0) {
        char *hdr_str = create_safe_string(hdr_data, hdr_len);
        if (hdr_str) {
            headers = curl_slist_append(headers, hdr_str);
            free(hdr_str);
        }
    }

    /* Also try appending a static dummy header to ensure list is not always empty */
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

    /* Call curl_mime_headers with take_ownership = 1 (let libcurl free) */
    if (headers) {
        curl_mime_headers(part, headers, 1);
    }

    /*
     * 2. Test curl_mime_encoder
     * Use the second segment as the encoding name.
     */
    const uint8_t *enc_data = Data + split1;
    size_t enc_len = split2 - split1;

    if (enc_len > 0) {
        char *enc_str = create_safe_string(enc_data, enc_len);
        if (enc_str) {
            curl_mime_encoder(part, enc_str);
            free(enc_str);
        }
    }

    /* Also test with NULL to remove encoding */
    curl_mime_encoder(part, NULL);

    /*
     * 3. Test curl_mime_name
     * Use the remaining data as the name.
     */
    const uint8_t *name_data = Data + split2;
    size_t name_len = Size - split2;

    if (name_len > 0) {
        char *name_str = create_safe_string(name_data, name_len);
        if (name_str) {
            curl_mime_name(part, name_str);
            free(name_str);
        }
    }

    /* Also test with NULL to clear name */
    curl_mime_name(part, NULL);

    /*
     * Additional exploration:
     * Create a new list manually to test curl_slist_append and curl_slist_free_all directly
     * without involving mime headers, to ensure the slist functions are hit independently.
     */
    struct curl_slist *test_list = NULL;
    char *test_str = create_safe_string(Data, Size);
    if (test_str) {
        test_list = curl_slist_append(test_list, test_str);
        free(test_str);
    }
    /* Append a few more to stress the list logic */
    test_list = curl_slist_append(test_list, "Test-Header: Value1");
    test_list = curl_slist_append(test_list, "Test-Header: Value2");

    /* Free the test list */
    curl_slist_free_all(test_list);

    /* Cleanup */
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}