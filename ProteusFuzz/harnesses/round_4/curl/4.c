// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Callback for curl_formget matching the expected signature:
   size_t (*curl_formget_callback)(void *ptr, const char *data, size_t size);
*/
static size_t formget_callback(void *ptr, const char *data, size_t size) {
    (void)ptr;
    (void)data;
    (void)size;
    /* Return the size to indicate success, or 0 to abort */
    return size;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    CURLFORMcode code;
    int ret;

    /* Prepare safe string buffers from fuzz input */
    /* Ensure we have at least 2 bytes to split into name and contents */
    if (Size < 2) {
        return 0;
    }

    const char *name = (const char *)Data;
    const char *contents = (const char *)(Data + 1);
    size_t name_len = Size / 2;
    size_t content_len = Size - name_len;

    char *safe_name = NULL;
    char *safe_contents = NULL;

    if (name_len > 0) {
        safe_name = malloc(name_len + 1);
        if (safe_name) {
            memcpy(safe_name, name, name_len);
            safe_name[name_len] = '\0';
        }
    }

    if (content_len > 0) {
        safe_contents = malloc(content_len + 1);
        if (safe_contents) {
            memcpy(safe_contents, contents, content_len);
            safe_contents[content_len] = '\0';
        }
    }

    /* Fuzz curl_formadd with various options */
    /* Try adding a simple text field */
    code = curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, safe_name ? safe_name : "field1",
                        CURLFORM_COPYCONTENTS, safe_contents ? safe_contents : "value1",
                        CURLFORM_END);

    /* Try adding another field with buffer */
    if (code == CURL_FORMADD_OK && Size > 10) {
        char *buffer = (char *)(Data + 10);
        size_t buf_len = Size - 10;
        code = curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "field2",
                            CURLFORM_BUFFER, "buffer_field",
                            CURLFORM_BUFFERPTR, buffer,
                            CURLFORM_BUFFERLENGTH, buf_len,
                            CURLFORM_END);
    }

    /* Try adding a field with filename flag (using dummy file) */
    if (code == CURL_FORMADD_OK) {
        code = curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "file_field",
                            CURLFORM_FILE, "./dummy_file",
                            CURLFORM_END);
    }

    /* Fuzz curl_formget */
    if (formpost) {
        ret = curl_formget(formpost, NULL, formget_callback);
        (void)ret;
    }

    /* Fuzz curl_strequal */
    if (safe_name && safe_contents) {
        int eq1 = curl_strequal(safe_name, safe_contents);
        int eq2 = curl_strequal(safe_name, NULL);
        int eq3 = curl_strequal(NULL, NULL);
        (void)eq1;
        (void)eq2;
        (void)eq3;
    }

    /* Fuzz curl_mprintf */
    /* Use a safe format string to avoid crashes from malformed input */
    curl_mprintf("Fuzzed data length: %zu\n", Size);

    /* Cleanup */
    if (formpost) {
        curl_formfree(formpost);
    }

    if (safe_name) {
        free(safe_name);
    }
    if (safe_contents) {
        free(safe_contents);
    }

    return 0;
}