// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type requirements */
static int dummy_push_callback(CURL *parent, CURL *easy, size_t num_headers,
                               struct curl_hentry *headers, void *userp) {
    (void)parent;
    (void)easy;
    (void)num_headers;
    (void)headers;
    (void)userp;
    return 1; /* Accept push */
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

/* Helper to create a dummy slist */
static struct curl_slist *create_dummy_slist(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    struct curl_slist *list = curl_slist_append(NULL, (const char *)data);
    if (size > 1) {
        char *next = malloc(size);
        if (next) {
            memcpy(next, data + 1, size - 1);
            next[size - 1] = '\0';
            list = curl_slist_append(list, next);
            free(next);
        }
    }
    return list;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi, easy);

    /* Fuzzing logic: Interpret input data to drive API calls */
    
    /* 1. Test curl_multi_setopt with CURLMOPT_PUSHFUNCTION */
    if (Size >= 4) {
        /* Use first 4 bytes to determine option and callback behavior */
        uint32_t option_val = *(const uint32_t *)Data;
        CURLMoption option = (CURLMoption)(option_val % 100 + 1);
        
        /* Try setting push function */
        if (option == CURLMOPT_PUSHFUNCTION) {
            curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
            curl_multi_setopt(multi, CURLMOPT_PUSHDATA, (void *)(uintptr_t)(option_val & 0xFF));
        }
        
        /* Try setting other common multi options based on input */
        if (option_val % 2 == 0) {
            long max_handles = (long)(option_val & 0xFFFF);
            curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, max_handles);
        }
        
        if (option_val % 3 == 0) {
            long pipe_depth = (long)(option_val & 0xFF);
            curl_multi_setopt(multi, CURLMOPT_PIPELINING, pipe_depth);
        }
    }

    /* 2. Test curl_easy_setopt with various options derived from input */
    if (Size >= 8) {
        uint32_t opt1 = *(const uint32_t *)(Data + 4);
        uint32_t opt2 = *(const uint32_t *)(Data + 8);

        /* Set URL if possible */
        if (Size > 12) {
            char url_buf[256];
            size_t url_len = (Size - 12) < 255 ? (Size - 12) : 255;
            memcpy(url_buf, Data + 12, url_len);
            url_buf[url_len] = '\0';
            curl_easy_setopt(easy, CURLOPT_URL, url_buf);
        }

        /* Set POST fields */
        if (opt1 % 5 == 0 && Size > 20) {
            size_t post_len = (opt2 & 0xFF);
            if (post_len > 0 && post_len < Size - 20) {
                curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, post_len);
                curl_easy_setopt(easy, CURLOPT_POSTFIELDS, (const char *)(Data + 20));
            }
        }

        /* Set headers (slist) */
        if (opt1 % 7 == 0) {
            struct curl_slist *headers = create_dummy_slist(Data, Size);
            if (headers) {
                curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
                curl_slist_free_all(headers);
            }
        }

        /* Set callbacks */
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, (void *)(uintptr_t)opt1);
        curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy, CURLOPT_READDATA, (void *)(uintptr_t)opt2);

        /* Set various long options */
        curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)(opt1 % 300));
        curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, (long)(opt2 % 60));
        curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, (long)(opt1 % 2));
        curl_easy_setopt(easy, CURLOPT_MAXREDIRS, (long)(opt2 % 10));
        
        /* Set error buffer */
        char errbuf[CURL_ERROR_SIZE];
        curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, errbuf);
    }

    /* 3. Perform a quick multi perform to exercise state */
    int running_handles = 0;
    CURLMcode mcode = curl_multi_perform(multi, &running_handles);
    (void)mcode; /* Ignore return for fuzzing, just exercise code path */

    /* 4. Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    return 0;
}