// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_nextheader at headers.c:121:21 in header.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_version_info at version.c:596:25 in curl.h
// curl_easy_header at headers.c:55:11 in header.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/header.h>
#include <curl/typecheck-gcc.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare environment: Initialize curl handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    /* Basic setup to make the handle somewhat valid for header queries later */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 100L);

    /* Fuzz curl_mprintf */
    /* Use data as format string with bounds checking to avoid crashes on invalid format */
    if (Size > 0) {
        char safe_fmt[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        memcpy(safe_fmt, Data, copy_len);
        safe_fmt[copy_len] = '\0';
        
        /* Ensure format string is safe by limiting to simple characters if needed,
           but mprintf is designed to be safe. We pass a dummy arg to avoid format errors. */
        curl_mprintf(safe_fmt, "fuzz");
    }

    /* Fuzz curl_msprintf */
    if (Size > 0) {
        char buffer[1024];
        char safe_fmt[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        memcpy(safe_fmt, Data, copy_len);
        safe_fmt[copy_len] = '\0';
        
        curl_msprintf(buffer, safe_fmt, "fuzz");
    }

    /* Fuzz curl_version_info */
    /* Iterate through possible CURLversion enums safely */
    for (int i = 0; i < 10; i++) {
        curl_version_info_data *info = curl_version_info((CURLversion)i);
        if (info) {
            /* Just access to ensure no crash, don't modify static data */
            (void)info->version;
            (void)info->features;
        }
    }

    /* Fuzz curl_easy_header and curl_easy_nextheader */
    /* These require a completed request or specific state. 
       We attempt to call them on the initialized handle. 
       They may return errors (CURLHE_NOREQUEST, etc.) which is expected and safe. */
    
    struct curl_header *hout = NULL;
    struct curl_header *prev = NULL;
    
    /* Use data as header name (ensure null termination) */
    if (Size > 0) {
        char header_name[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        memcpy(header_name, Data, copy_len);
        header_name[copy_len] = '\0';

        /* Try various indices and origins */
        for (size_t idx = 0; idx < 5; idx++) {
            for (unsigned int origin = 0; origin < 4; origin++) {
                for (int req_id = -1; req_id <= 2; req_id++) {
                    curl_easy_header(easy, header_name, idx, origin, req_id, &hout);
                }
            }
        }
        
        /* Iterate headers */
        for (unsigned int origin = 0; origin < 4; origin++) {
            for (int req_id = -1; req_id <= 2; req_id++) {
                prev = curl_easy_nextheader(easy, origin, req_id, prev);
                /* If we got a header, try to read fields safely */
                if (prev) {
                    (void)prev->name;
                    (void)prev->value;
                    (void)prev->amount;
                    (void)prev->index;
                    (void)prev->origin;
                }
            }
        }
    }

    /* Fuzz type checking macros by invoking curl_easy_setopt with various data types */
    /* We use the fuzz input to generate option values */
    long long_val = (long)Data[0];
    curl_off_t off_val = (curl_off_t)Data[0];
    char *str_val = (char *)Data;
    
    /* Set some options that accept long */
    curl_easy_setopt(easy, CURLOPT_VERBOSE, long_val);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, long_val);
    
    /* Set options that accept curl_off_t */
    curl_easy_setopt(easy, CURLOPT_MAX_RECV_SPEED_LARGE, off_val);
    
    /* Set options that accept strings (ensure null termination if we modify) */
    /* We use the raw data as a string for options that expect char* */
    /* Note: curl_easy_setopt expects a null-terminated string for most string options.
       If Data is not null-terminated, curl might read past buffer. 
       To be safe, we only pass if we can ensure termination or use a safe buffer. */
    if (Size > 0 && Data[Size-1] == 0) {
        curl_easy_setopt(easy, CURLOPT_USERAGENT, (char *)Data);
    } else {
        /* Create a safe string from data */
        char safe_str[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        memcpy(safe_str, Data, copy_len);
        safe_str[copy_len] = '\0';
        curl_easy_setopt(easy, CURLOPT_USERAGENT, safe_str);
    }

    /* Cleanup */
    curl_easy_cleanup(easy);

    /* Write dummy file if needed (optional, but per instructions if code requires file) */
    write_dummy_file(Data, Size);

    return 0;
}