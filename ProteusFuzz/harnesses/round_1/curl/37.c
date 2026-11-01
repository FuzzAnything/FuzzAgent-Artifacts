// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback for curl_formget matching the expected signature */
static size_t dummy_formget_callback(void *arg, const char *ptr, size_t size) {
    (void)arg;
    (void)ptr;
    (void)size;
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_formadd and curl_formfree */
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;

    /* Create a buffer from input data to use as form content */
    char *content = (char *)malloc(Size + 1);
    if (content) {
        memcpy(content, Data, Size);
        content[Size] = '\0';
    }

    /* Attempt to add a form part using input data */
    /* We use a safe subset of options to avoid crashes from malformed input */
    if (content) {
        curl_formadd(&form, &last,
                     CURLFORM_COPYNAME, "fuzz_data",
                     CURLFORM_COPYCONTENTS, content,
                     CURLFORM_END);
    }

    /* Try adding another part with a different flag to explore states */
    if (content && Size > 0) {
        char *name_buf = (char *)malloc(Size + 1);
        if (name_buf) {
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
            curl_formadd(&form, &last,
                         CURLFORM_COPYNAME, name_buf,
                         CURLFORM_BUFFER, "buffer_data",
                         CURLFORM_BUFFERPTR, content,
                         CURLFORM_BUFFERLENGTH, Size,
                         CURLFORM_END);
            free(name_buf);
        }
    }

    /* Fuzz curl_formget if form exists */
    if (form) {
        curl_formget(form, NULL, dummy_formget_callback);
    }

    /* Fuzz curl_mprintf with input data as format string (carefully) */
    /* We limit the format string to avoid undefined behavior with %s etc. */
    /* Using a safe format string pattern or just printing the data as hex */
    if (Size > 0) {
        /* Safe way: print a fixed format with a small portion of data if printable */
        char safe_char = (Data[0] >= 32 && Data[0] < 127) ? Data[0] : '?';
        curl_mprintf("Fuzz char: %c\n", safe_char);
        
        /* Also try to use the data as a string if it looks like text */
        if (Size < 1024) {
            char *str = (char *)malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = '\0';
                /* Only print if it doesn't contain nulls in the middle (already handled by malloc copy) */
                /* curl_mprintf is safe, but we avoid %s with untrusted data to prevent format string issues */
                curl_mprintf("Data length: %zu\n", Size);
                free(str);
            }
        }
    }

    /* Cleanup form */
    if (form) {
        curl_formfree(form);
    }

    /* Cleanup content buffer */
    if (content) {
        free(content);
    }

    /* Cleanup curl handle */
    curl_easy_cleanup(curl);

    /* Cleanup global */
    curl_global_cleanup();

    return 0;
}