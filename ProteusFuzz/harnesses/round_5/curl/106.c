// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

/* Dummy callback implementations to satisfy type checking macros */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy; (void)s; (void)what; (void)userp; (void)socketp;
    return 0;
}

/* curl_time_t is typically long long or long, depending on the platform. 
   We use long long to be safe, or cast if necessary. 
   In modern curl, it is defined in curl.h, but if missing in some environments, 
   we define a fallback or use long long. */
#ifndef curl_time_t
typedef long long curl_time_t;
#endif

static int dummy_timer_callback(CURLM *multi, curl_time_t timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

static int dummy_conv_callback(char *in, size_t inlen, char *out, size_t outlen, size_t *outwritten) {
    (void)in; (void)inlen; (void)out; (void)outlen; (void)outwritten;
    return 0;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Set a dummy URL to avoid immediate failures in some contexts */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void*)Data);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void*)Data);
        
        /* Add to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* Fuzz curl_multi_socket */
    size_t offset = 0;
    if (Size > 0) {
        curl_socket_t s = (curl_socket_t)Data[offset];
        int running_handles = 0;
        /* Invoke deprecated curl_multi_socket */
        curl_multi_socket(multi_handle, s, &running_handles);
    }

    /* Fuzz curl_multi_socket_all */
    if (Size > 1) {
        int running_handles = 0;
        /* Invoke deprecated curl_multi_socket_all */
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    /* Fuzz curl_multi_setopt with various callback types to trigger type check macros */
    if (Size >= 4) {
        /* Test socket callback */
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, (void*)(Data + 4));

        /* Test timer callback */
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, (void*)(Data + 5));
    }

    /* Fuzz curl_easy_setopt with various callback types on the easy handle */
    if (easy_handle && Size >= 8) {
        /* Test conv callbacks */
        curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
        curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
        curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
        
        /* Test slist option (Wcurl_easy_setopt_err_curl_slist) */
        struct curl_slist *slist = NULL;
        if (Size > 10) {
            slist = curl_slist_append(slist, (const char*)(Data + 8));
            slist = curl_slist_append(slist, "Test-Header: Value");
        }
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, slist);
        if (slist) curl_slist_free_all(slist);

        /* Test cb_data options (Wcurl_easy_setopt_err_cb_data) */
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void*)(Data + 10));
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void*)(Data + 11));
        curl_easy_setopt(easy_handle, CURLOPT_DEBUGDATA, (void*)(Data + 12));
        
        /* Test long options (Wcurl_easy_setopt_err_long) */
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)Data[0]);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, (long)Data[1]);

        /* Test string options (Wcurl_easy_setopt_err_string) */
        if (Size > 20) {
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, (char*)(Data + 20));
        }
    }

    /* Cleanup */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}