// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

/* Callback for curl_formget matching the expected signature */
static size_t formget_callback(void *arg, const char *buffer, size_t size) {
    (void)arg;
    (void)buffer;
    (void)size;
    /* Simple callback that always succeeds */
    return 0;
}

/* Helper to extract a string from fuzz input */
static char *extract_string(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return NULL;
    
    size_t len = 0;
    while (*offset < size && len < 1024) {
        if (data[*offset] == 0) break;
        len++;
        (*offset)++;
    }
    
    if (len == 0) return NULL;
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + *offset - len, len);
    str[len] = '\0';
    return str;
}

/* Helper to extract a length from fuzz input */
static long extract_length(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) return 0;
    
    long len = 0;
    len = (long)data[*offset] | ((long)data[*offset + 1] << 8) |
          ((long)data[*offset + 2] << 16) | ((long)data[*offset + 3] << 24);
    
    *offset += 4;
    return len;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    struct curl_httppost *form = NULL;
    struct curl_httppost *last_post = NULL;
    CURLFORMcode code;
    int result;
    char buffer[1024];
    
    /* Initialize curl */
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    /* Test curl_formadd with various options */
    size_t offset = 0;
    
    /* Try to build a form based on input data */
    if (Size > 10) {
        char *name = extract_string(Data, &offset, Size);
        char *contents = extract_string(Data, &offset, Size);
        long contentslength = extract_length(Data, &offset, Size);
        
        if (name && contents) {
            /* Add a simple text field */
            code = curl_formadd(&form, &last_post,
                               CURLFORM_COPYNAME, name,
                               CURLFORM_COPYCONTENTS, contents,
                               CURLFORM_CONTENTSLENGTH, contentslength,
                               CURLFORM_END);
            
            free(name);
            free(contents);
        } else {
            if (name) free(name);
            if (contents) free(contents);
        }
        
        /* Try adding a buffer field */
        if (Size > offset + 20) {
            char *buffer_name = extract_string(Data, &offset, Size);
            if (buffer_name) {
                code = curl_formadd(&form, &last_post,
                                   CURLFORM_COPYNAME, buffer_name,
                                   CURLFORM_BUFFER, "test_buffer",
                                   CURLFORM_BUFFERLENGTH, 11,
                                   CURLFORM_END);
                free(buffer_name);
            }
        }
        
        /* Try adding a file field (will fail gracefully with dummy file) */
        if (Size > offset + 10) {
            char *file_name = extract_string(Data, &offset, Size);
            if (file_name) {
                code = curl_formadd(&form, &last_post,
                                   CURLFORM_COPYNAME, file_name,
                                   CURLFORM_FILE, "./dummy_file",
                                   CURLFORM_END);
                free(file_name);
            }
        }
    }
    
    /* Test curl_formget if form was created */
    if (form) {
        result = curl_formget(form, NULL, formget_callback);
        (void)result;
    }
    
    /* Test curl_mprintf */
    if (Size > 5) {
        char *format_str = extract_string(Data, &offset, Size);
        if (format_str) {
            result = curl_mprintf("%s", format_str);
            (void)result;
            free(format_str);
        }
    }
    
    /* Test curl_msprintf */
    if (Size > 10) {
        char *format_str2 = extract_string(Data, &offset, Size);
        if (format_str2) {
            result = curl_msprintf(buffer, format_str2, "test");
            (void)result;
            free(format_str2);
        }
    }
    
    /* Test curl_strequal with various inputs */
    if (Size > 20) {
        char *str1 = extract_string(Data, &offset, Size);
        char *str2 = extract_string(Data, &offset, Size);
        
        if (str1 && str2) {
            result = curl_strequal(str1, str2);
            (void)result;
            free(str1);
            free(str2);
        } else {
            if (str1) free(str1);
            if (str2) free(str2);
        }
        
        /* Test with NULL */
        result = curl_strequal(NULL, NULL);
        (void)result;
    }
    
    /* Test curl_formfree - this is the main target */
    if (form) {
        curl_formfree(form);
        form = NULL;
    }
    
    /* Cleanup */
    curl_easy_cleanup(curl);
    
    return 0;
}