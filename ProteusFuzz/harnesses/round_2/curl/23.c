// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback for formget to consume data.
 * Signature must match curl_formget_callback: size_t (*)(void *, const char *, size_t)
 */
static size_t dummy_formget_callback(void *userptr, const char *arg, size_t len) {
    (void)userptr;
    (void)arg;
    (void)len;
    return len;
}

/* Dummy read callback for CURLOPT_READFUNCTION */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* 1. Test curl_formadd and curl_formfree */
    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;
    
    if (Size > 0) {
        /* Use fuzzed data as content for the form field */
        CURLFORMcode code = curl_formadd(&post, &last,
            CURLFORM_COPYNAME, "fuzz_field",
            CURLFORM_COPYCONTENTS, (char *)Data,
            CURLFORM_CONTENTSLENGTH, (long)Size,
            CURLFORM_END);
        
        if (code == CURL_FORMADD_OK && post != NULL) {
            /* Test curl_formget with the created form */
            int get_result = curl_formget(post, NULL, dummy_formget_callback);
            (void)get_result;
            
            /* Clean up the form */
            curl_formfree(post);
            post = NULL;
        }
    }

    /* 2. Test curl_formadd with NULL to ensure robustness */
    curl_formadd(NULL, NULL, CURLFORM_END);

    /* 3. Test curl_formfree with NULL */
    curl_formfree(NULL);

    /* 4. Test curl_mprintf with fuzzed format string */
    char safe_format[256];
    size_t copy_len = (Size < sizeof(safe_format) - 1) ? Size : sizeof(safe_format) - 1;
    memcpy(safe_format, Data, copy_len);
    safe_format[copy_len] = '\0';
    
    /* Print using %s to safely handle the fuzzed string as data */
    int mprintf_result = curl_mprintf("%s", safe_format);
    (void)mprintf_result;

    /* 5. Test curl_easy_setopt with various options */
    CURL *handle = curl_easy_init();
    if (handle) {
        /* Test with a string option */
        curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
        
        /* Test with a long option */
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10L);
        
        /* Test with a callback option */
        curl_easy_setopt(handle, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(handle, CURLOPT_READDATA, NULL);
        
        /* Test with a struct curl_httppost option */
        curl_easy_setopt(handle, CURLOPT_HTTPPOST, NULL);
        
        /* Test with a custom data pointer */
        curl_easy_setopt(handle, CURLOPT_PRIVATE, (void *)Data);
        
        curl_easy_cleanup(handle);
    }

    /* 6. Test curl_formadd with invalid/edge cases */
    struct curl_httppost *post2 = NULL;
    struct curl_httppost *last2 = NULL;
    
    /* Attempt to add a form with NULL name */
    curl_formadd(&post2, &last2,
        CURLFORM_COPYNAME, NULL,
        CURLFORM_COPYCONTENTS, "test",
        CURLFORM_END);
    if (post2) curl_formfree(post2);

    /* Attempt to add a form with NULL contents */
    curl_formadd(&post2, &last2,
        CURLFORM_COPYNAME, "test_name",
        CURLFORM_COPYCONTENTS, NULL,
        CURLFORM_END);
    if (post2) curl_formfree(post2);

    /* Cleanup global curl */
    curl_global_cleanup();

    return 0;
}