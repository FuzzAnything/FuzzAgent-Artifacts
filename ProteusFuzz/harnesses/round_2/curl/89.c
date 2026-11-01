// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy data for fuzzing\n");
        fclose(f);
    }
}

/* Dummy callback implementations to satisfy type checks if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a dummy file for file-based options if needed */
    create_dummy_file();

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Prepare fuzz data as a string for URL or other string options */
    char *fuzz_str = NULL;
    if (Size > 0) {
        fuzz_str = malloc(Size + 1);
        if (fuzz_str) {
            memcpy(fuzz_str, Data, Size);
            fuzz_str[Size] = '\0';
        }
    }

    /* Set various options using fuzz data to explore different states */
    if (fuzz_str) {
        /* Fuzz URL */
        curl_easy_setopt(easy_handle, CURLOPT_URL, fuzz_str);
        
        /* Fuzz User-Agent */
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, fuzz_str);
        
        /* Fuzz Referer */
        curl_easy_setopt(easy_handle, CURLOPT_REFERER, fuzz_str);
        
        /* Fuzz Cookie */
        curl_easy_setopt(easy_handle, CURLOPT_COOKIE, fuzz_str);
        
        /* Fuzz Proxy */
        curl_easy_setopt(easy_handle, CURLOPT_PROXY, fuzz_str);
        
        /* Fuzz CAInfo */
        curl_easy_setopt(easy_handle, CURLOPT_CAINFO, fuzz_str);
        
        /* Fuzz SSLCert */
        curl_easy_setopt(easy_handle, CURLOPT_SSLCERT, fuzz_str);
        
        /* Fuzz PostFields */
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, fuzz_str);
        
        /* Fuzz CustomRequest */
        curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, fuzz_str);
    }

    /* Set callbacks */
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);

    /* Set some long options using fuzz data as integers */
    long fuzz_long = 0;
    if (Size >= sizeof(long)) {
        memcpy(&fuzz_long, Data, sizeof(long));
    } else if (Size > 0) {
        fuzz_long = (long)Data[0];
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, fuzz_long & 1);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, (fuzz_long >> 1) & 1);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (fuzz_long >> 2) & 0xFF);
    curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, (fuzz_long >> 10) & 0xFF);

    /* Set multi options - Fixed: Use CURLMOPT_ prefix for multi options */
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (fuzz_long >> 20) & 0xFF);
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (fuzz_long >> 30) & 1);

    /* Perform multi operations */
    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    
    /* Fuzz curl_multi_perform */
    curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket (deprecated but still valid for fuzzing) */
    curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    
    /* Fuzz curl_multi_socket_all (deprecated but still valid for fuzzing) */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    if (fuzz_str) {
        free(fuzz_str);
    }

    curl_global_cleanup();

    return 0;
}