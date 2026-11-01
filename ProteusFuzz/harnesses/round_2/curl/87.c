// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_easy_unescape at escape.c:163:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl library functions */

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a CURL handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare a dummy file for file-based operations */
    const char *dummy_file = "./dummy_file";
    write_dummy_file(dummy_file, Data, Size);

    /* Test curl_mfprintf and curl_mvfprintf */
    FILE *fd = fopen(dummy_file, "w");
    if (fd) {
        /* Test curl_mfprintf with various format strings derived from input */
        if (Size >= 2) {
            char format_buf[256];
            size_t fmt_len = (Size < 255) ? Size : 255;
            memcpy(format_buf, Data, fmt_len);
            format_buf[fmt_len] = '\0';
            
            /* Ensure format string is safe by limiting to simple patterns */
            if (strstr(format_buf, "%s") || strstr(format_buf, "%d") || strstr(format_buf, "%x")) {
                curl_mfprintf(fd, format_buf, (int)Data[0], (int)Data[1]);
            } else {
                /* Fallback safe format */
                /* Ensure the string passed to %s is null-terminated */
                char safe_str[256];
                size_t copy_len = (Size < 255) ? Size : 255;
                memcpy(safe_str, Data, copy_len);
                safe_str[copy_len] = '\0';
                curl_mfprintf(fd, "Test data: %s", safe_str);
            }
        }

        /* Test curl_mvfprintf logic via curl_mfprintf with different args */
        if (Size >= 2) {
            char format_buf2[256];
            size_t fmt_len2 = (Size < 255) ? Size : 255;
            memcpy(format_buf2, Data, fmt_len2);
            format_buf2[fmt_len2] = '\0';
            
            if (strstr(format_buf2, "%s") || strstr(format_buf2, "%d")) {
                curl_mfprintf(fd, format_buf2, (int)Data[0], (int)Data[1]);
            } else {
                /* Ensure the string passed to %s is null-terminated */
                char safe_str2[256];
                size_t copy_len2 = (Size < 255) ? Size : 255;
                memcpy(safe_str2, Data, copy_len2);
                safe_str2[copy_len2] = '\0';
                curl_mfprintf(fd, "Test data: %s", safe_str2);
            }
        }
        fclose(fd);
    }

    /* Test curl_easy_unescape */
    if (Size > 0) {
        char *unescaped = NULL;
        int outlength = 0;
        
        /* Ensure the input is a valid C string for unescape */
        char *input_str = (char *)malloc(Size + 1);
        if (input_str) {
            memcpy(input_str, Data, Size);
            input_str[Size] = '\0';
            
            /* Try unescaping the entire buffer */
            unescaped = curl_easy_unescape(handle, input_str, Size, &outlength);
            
            if (unescaped) {
                free(unescaped);
            }
            
            /* Try unescaping with length 0 */
            unescaped = curl_easy_unescape(handle, input_str, 0, NULL);
            if (unescaped) {
                free(unescaped);
            }
            
            /* Try with NULL string */
            unescaped = curl_easy_unescape(handle, NULL, 0, NULL);
            if (unescaped) {
                free(unescaped);
            }
            
            free(input_str);
        }
    }

    /* Test curl_easy_setopt with various option types to trigger type checking */
    
    /* String option */
    if (Size > 0) {
        char *str_val = (char *)malloc(Size + 1);
        if (str_val) {
            memcpy(str_val, Data, Size);
            str_val[Size] = '\0';
            curl_easy_setopt(handle, CURLOPT_URL, str_val);
            curl_easy_setopt(handle, CURLOPT_USERAGENT, str_val);
            free(str_val);
        }
    }

    /* Long option */
    if (Size >= sizeof(long)) {
        long long_val = *(long *)Data;
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, long_val);
        curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, long_val);
    }

    /* slist option */
    struct curl_slist *slist = NULL;
    if (Size > 0) {
        char *header = (char *)malloc(Size + 1);
        if (header) {
            memcpy(header, Data, Size);
            header[Size] = '\0';
            slist = curl_slist_append(slist, header);
            free(header);
            
            if (slist) {
                curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);
                curl_slist_free_all(slist);
            }
        }
    }

    /* Clean up */
    curl_easy_cleanup(handle);
    curl_global_cleanup();
    
    /* Remove dummy file */
    remove(dummy_file);

    return 0;
}