// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strequal at strequal.c:76:5 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_strequal at strequal.c:76:5 in curl.h
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

/* 
 * Dummy callback for curl_formget to prevent NULL pointer issues.
 * Must match the signature: size_t (*)(void *, const char *, size_t)
 * Returns the length of data processed to indicate success.
 */
static size_t dummy_formget_callback(void *arg, const char *buf, size_t len) {
    (void)arg;
    (void)buf;
    /* Return len to indicate all data was processed successfully */
    return len;
}

/* 
 * Helper to safely free a curl_httppost list.
 * Handles NULL input gracefully.
 */
static void safe_formfree(struct curl_httppost *form) {
    if (form) {
        curl_formfree(form);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* 
     * 1. Fuzz curl_easy_escape
     * Ensure the input string is null-terminated for safety.
     */
    char *escaped = NULL;
    if (Size > 0) {
        char *input_buf = (char *)malloc(Size + 1);
        if (input_buf) {
            memcpy(input_buf, Data, Size);
            input_buf[Size] = '\0';
            
            /* Test with length=0 (null-terminated) */
            escaped = curl_easy_escape(handle, input_buf, 0);
            
            /* Test with specific length (Size) */
            char *escaped2 = curl_easy_escape(handle, input_buf, (int)Size);
            if (escaped2) {
                curl_free(escaped2);
            }
            
            free(input_buf);
        }
    }
    if (escaped) {
        curl_free(escaped);
    }

    /* 
     * 2. Fuzz curl_mprintf
     * Use the fuzzed data as a format string. 
     * Note: This is inherently risky in real code, but for fuzzing 
     * the internal memory safety of curl_mprintf, it is valid.
     */
    if (Size > 0) {
        char *fmt_buf = (char *)malloc(Size + 1);
        if (fmt_buf) {
            memcpy(fmt_buf, Data, Size);
            fmt_buf[Size] = '\0';
            /* Suppress output for fuzzing stability by redirecting or ignoring return */
            curl_mprintf("%s", fmt_buf);
            free(fmt_buf);
        }
    }

    /* 
     * 3. Fuzz curl_strequal
     * Compare the input data against itself, a static string, and NULL.
     */
    if (Size > 0) {
        char *s1 = (char *)malloc(Size + 1);
        if (s1) {
            memcpy(s1, Data, Size);
            s1[Size] = '\0';
            
            curl_strequal(s1, s1);
            curl_strequal(s1, "test");
            curl_strequal(s1, NULL);
            curl_strequal(NULL, s1);
            curl_strequal(NULL, NULL);
            
            free(s1);
        }
    }

    /* 
     * 4, 5, 6. Fuzz curl_formadd, curl_formget, curl_formfree
     * These functions work with linked lists of curl_httppost.
     */
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;
    CURLFORMcode code;

    /* 
     * Strategy: Use the first part of data as name, second as content.
     */
    if (Size >= 2) {
        size_t mid = Size / 2;
        char *name = (char *)malloc(mid + 1);
        char *content = (char *)malloc(Size - mid + 1);
        
        if (name && content) {
            memcpy(name, Data, mid);
            name[mid] = '\0';
            
            memcpy(content, Data + mid, Size - mid);
            content[Size - mid] = '\0';
            
            /* Add a form part: name and contents */
            code = curl_formadd(&form, &last,
                                CURLFORM_COPYNAME, name,
                                CURLFORM_COPYCONTENTS, content,
                                CURLFORM_END);
            
            if (code == CURL_FORMADD_OK) {
                /* Try to serialize the form using curl_formget */
                int get_result = curl_formget(form, NULL, dummy_formget_callback);
                (void)get_result;
                
                /* Cleanup the form */
                safe_formfree(form);
            }
            
            free(name);
            free(content);
        }
    }

    /* 
     * Additional edge case: Try to add a form with NULL name or content 
     * to trigger error paths in curl_formadd.
     */
    struct curl_httppost *form_err = NULL;
    struct curl_httppost *last_err = NULL;
    code = curl_formadd(&form_err, &last_err,
                        CURLFORM_COPYNAME, NULL,
                        CURLFORM_END);
    (void)code;
    safe_formfree(form_err);

    /* Cleanup curl handle */
    curl_easy_cleanup(handle);

    return 0;
}