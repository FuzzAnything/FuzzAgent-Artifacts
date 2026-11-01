// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formget at formdata.c:627:5 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

/* 
 * curl_formget_callback expects: size_t (*)(void *arg, const char *buf, size_t len)
 * It returns the number of bytes processed (usually len) or 0 to abort.
 */
static size_t dummy_formget_callback(void *arg, const char *buf, size_t len) {
    (void)arg;
    (void)buf;
    (void)len;
    /* Return len to indicate success, or 0 to abort */
    return len;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Fuzz curl_easy_unescape
    // Data[0] determines length strategy
    int length = (Data[0] % (Size - 1));
    
    // Create a temporary buffer with a guaranteed null terminator.
    // We copy up to 'length' bytes or 'Size-1' bytes, whichever is smaller, 
    // ensuring we don't read past the input buffer and always have a null terminator.
    size_t safe_len = (size_t)length;
    if (safe_len > Size - 1) {
        safe_len = Size - 1;
    }
    
    char *safe_string = malloc(safe_len + 1);
    if (!safe_string) {
        curl_easy_cleanup(handle);
        return 0;
    }
    
    if (safe_len > 0) {
        memcpy(safe_string, Data + 1, safe_len);
    }
    safe_string[safe_len] = '\0';

    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, safe_string, length, &outlength);
    if (unescaped) {
        // Simulate usage
        if (outlength > 0) {
            char dummy_buf[256];
            size_t copy_len = (outlength < 255) ? (size_t)outlength : 255;
            memcpy(dummy_buf, unescaped, copy_len);
            dummy_buf[copy_len] = '\0';
            curl_mprintf("%s", dummy_buf);
        }
        curl_free(unescaped);
    }
    
    free(safe_string);

    // Fuzz curl_formadd and curl_formfree
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;
    
    // Use remaining data to construct form parts
    size_t offset = 1;
    
    // Try to add a simple string part
    if (Size > 2) {
        size_t name_len = Data[offset] % (Size - offset - 1);
        if (name_len > 0 && name_len < (Size - offset - 1)) {
            char *name = (char *)(Data + offset + 1);
            char *value = (char *)(Data + offset + 1 + name_len);
            size_t value_len = (Size - offset - 1 - name_len);
            
            // Ensure null termination for safety in this context
            char *name_copy = malloc(name_len + 1);
            char *value_copy = malloc(value_len + 1);
            if (name_copy && value_copy) {
                memcpy(name_copy, name, name_len);
                name_copy[name_len] = '\0';
                memcpy(value_copy, value, value_len);
                value_copy[value_len] = '\0';
                
                curl_formadd(&form, &last,
                             CURLFORM_COPYNAME, name_copy,
                             CURLFORM_COPYCONTENTS, value_copy,
                             CURLFORM_END);
                free(name_copy);
                free(value_copy);
            }
        }
    }

    // Fuzz curl_formget if form exists
    if (form) {
        curl_formget(form, NULL, dummy_formget_callback);
    }

    // Fuzz curl_msprintf
    // Ensure the format string and arguments are null-terminated to prevent buffer overflows
    char buffer[512];
    if (Size > 2) {
        // Construct a simple format string from input, ensuring safety
        // We limit format specifiers to avoid crashes and ensure null termination
        char format[64];
        format[0] = '%';
        if (Data[Size-1] < 10) {
            format[1] = 'd';
        } else if (Data[Size-1] < 20) {
            format[1] = 'x';
        } else {
            format[1] = 's';
        }
        format[2] = '\0';
        
        // For %s, we must provide a null-terminated string.
        // We create a temporary buffer for the string argument.
        size_t arg_len = Size - 2; // Exclude the byte used for format selection and the first byte
        if (arg_len > 255) arg_len = 255;
        
        char *arg_str = malloc(arg_len + 1);
        if (arg_str) {
            memcpy(arg_str, Data + 1, arg_len);
            arg_str[arg_len] = '\0';
            
            int ret = curl_msprintf(buffer, format, arg_str);
            (void)ret; // suppress unused warning
            free(arg_str);
        }
    }

    // Cleanup
    if (form) {
        curl_formfree(form);
    }

    curl_easy_cleanup(handle);

    return 0;
}