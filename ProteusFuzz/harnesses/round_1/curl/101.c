// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback to satisfy type requirements if needed */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Initialize global curl environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to associate with multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Prepare a dummy file for file-based options if needed */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        /* Write a copy of the data to the file, do not modify Data */
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Fuzz curl_multi_setopt */
    /* We try various options, using data from the input where possible */
    if (Size > 0) {
        int option_val = Data[0];
        /* Try setting a few common multi options safely */
        if (option_val == 0) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)Size);
        } else if (option_val == 1) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Size % 100));
        } else if (option_val == 2) {
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Size % 3));
        } else {
            /* Try setting a generic long option if within safe range */
            if (option_val < 10) {
                curl_multi_setopt(multi_handle, (CURLMoption)option_val, (long)Size);
            }
        }
    }

    /* Fuzz curl_multi_remove_handle */
    /* Add easy handle to multi first to make removal meaningful */
    curl_multi_add_handle(multi_handle, easy_handle);
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Fuzz curl_multi_strerror */
    /* Generate various error codes from input */
    for (size_t i = 0; i < Size && i < 20; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *msg = curl_multi_strerror(code);
        if (msg) {
            /* Use the string to prevent unused variable warnings and explore state */
            volatile size_t len = strlen(msg);
            (void)len;
        }
    }

    /* Fuzz curl_easy_setopt with various types using the Wcurl_* macros implicitly */
    /* We set options on the easy_handle to trigger type checking logic */
    
    /* String option: Create a mutable copy to ensure null termination without modifying Data */
    char *str_data = NULL;
    if (Size > 0) {
        str_data = (char *)malloc(Size + 1);
        if (str_data) {
            memcpy(str_data, Data, Size);
            str_data[Size] = '\0';
            
            curl_easy_setopt(easy_handle, CURLOPT_URL, str_data);
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, str_data);
        }
    }

    /* Long option */
    if (Size > 1) {
        long long_val = (long)(Data[0] << 8) | Data[1];
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, long_val);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, long_val);
    }

    /* Callback option */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    /* Pass a copy or a safe pointer for WRITEDATA, avoiding const violation if Data is const */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)str_data);

    /* Error buffer option */
    char error_buffer[CURL_ERROR_SIZE];
    curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buffer);

    /* File option */
    if (dummy_file) {
        /* Reopen for reading if needed, or just use the path */
        curl_easy_setopt(easy_handle, CURLOPT_CAINFO, "./dummy_file");
    }

    /* Cleanup allocated memory */
    free(str_data);

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global */
    curl_global_cleanup();

    return 0;
}