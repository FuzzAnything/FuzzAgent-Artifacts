// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_strequal at strequal.c:76:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static size_t formget_callback(void *arg, const char *ptr, size_t size) {
    (void)arg;
    (void)ptr;
    (void)size;
    return size;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test curl_mprintf with fuzzed format string
    // Limit size to prevent overflow in format string
    size_t fmt_len = Size > 256 ? 256 : Size;
    char *fmt_buf = malloc(fmt_len + 1);
    if (!fmt_buf) return 0;
    memcpy(fmt_buf, Data, fmt_len);
    fmt_buf[fmt_len] = '\0';
    
    // Ensure format string is safe for printf-like functions
    // Replace any '%' that isn't followed by a valid specifier to avoid crashes
    for (size_t i = 0; i < fmt_len; i++) {
        if (fmt_buf[i] == '%') {
            if (i + 1 >= fmt_len || !strchr("sdiouxXeEfFgGaAn%", fmt_buf[i+1])) {
                fmt_buf[i] = 'x'; // Replace with safe char
            }
        }
    }
    
    int mprintf_ret = curl_mprintf(fmt_buf, "fuzz_data");
    (void)mprintf_ret;
    free(fmt_buf);

    // Test curl_msprintf
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    // Create a null-terminated string from fuzzed data for safe usage with %s
    size_t safe_data_len = Size > 512 ? 512 : Size;
    char *safe_data = malloc(safe_data_len + 1);
    if (safe_data) {
        memcpy(safe_data, Data, safe_data_len);
        safe_data[safe_data_len] = '\0';
        int msprintf_ret = curl_msprintf(buffer, "%s", safe_data);
        (void)msprintf_ret;
        free(safe_data);
    }

    // Test curl_strequal
    const char *s1 = NULL;
    const char *s2 = NULL;
    
    // Ensure null termination for string comparison safety
    char safe_s1[64] = {0};
    char safe_s2[64] = {0};
    
    if (Size > 10) {
        size_t len1 = strnlen((const char *)(Data + 10), 63);
        memcpy(safe_s1, Data + 10, len1);
        s1 = safe_s1;
    }
    
    if (Size > 20) {
        size_t len2 = strnlen((const char *)(Data + 20), 63);
        memcpy(safe_s2, Data + 20, len2);
        s2 = safe_s2;
    }
    
    int strequal_ret = curl_strequal(s1, s2);
    (void)strequal_ret;

    // Test curl_formadd, curl_formget, curl_formfree
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;
    
    // Construct form parts based on input data
    // We use small chunks to avoid excessive memory usage
    if (Size > 5) {
        char name[32] = {0};
        char contents[64] = {0};
        
        size_t n_len = (Size > 10) ? (Size % 10) : 5;
        size_t c_len = (Size > 20) ? (Size % 20) : 10;
        
        // Ensure we don't read past the buffer
        if (n_len > 0 && n_len < 32 && n_len <= Size) {
            memcpy(name, Data, n_len);
        }
        // Ensure we don't read past the buffer for contents
        if (c_len > 0 && c_len < 64 && c_len <= Size) {
            // Start reading from Data[0] or Data[1] but ensure bounds
            // Using Data directly with length check
            memcpy(contents, Data, c_len);
        }

        CURLFORMcode code = curl_formadd(&form, &last,
            CURLFORM_COPYNAME, name,
            CURLFORM_COPYCONTENTS, contents,
            CURLFORM_END);
        
        if (code == CURL_FORMADD_OK) {
            // Try to add another part
            if (Size > 100) {
                char name2[32] = {0};
                char contents2[64] = {0};
                size_t n2_len = (Size % 10) + 1;
                size_t c2_len = (Size % 20) + 1;
                
                // Ensure bounds for second part
                if (n2_len < 32 && n2_len <= Size) {
                    memcpy(name2, Data, n2_len);
                }
                if (c2_len < 64 && c2_len <= Size) {
                    memcpy(contents2, Data, c2_len);
                }
                
                code = curl_formadd(&form, &last,
                    CURLFORM_COPYNAME, name2,
                    CURLFORM_COPYCONTENTS, contents2,
                    CURLFORM_END);
            }
        }
        
        if (form) {
            // Test curl_formget
            int formget_ret = curl_formget(form, NULL, formget_callback);
            (void)formget_ret;
            
            // Cleanup
            curl_formfree(form);
        }
    }

    return 0;
}