// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Prepare dummy file if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Initialize curl_easy_setopt with various options to trigger type checks */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Fuzz curl_multi_socket */
    if (Size >= sizeof(curl_socket_t)) {
        curl_socket_t sock;
        memcpy(&sock, Data, sizeof(curl_socket_t));
        int running_handles = 0;
        curl_multi_socket(multi_handle, sock, &running_handles);
    }

    /* Fuzz curl_multi_socket_all */
    if (Size >= sizeof(int)) {
        int running_handles = 0;
        memcpy(&running_handles, Data, sizeof(int));
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    /* Fuzz curl_multi_setopt with socket callback */
    if (Size >= sizeof(void *)) {
        void *socket_cb_data = (void *)Data;
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, socket_cb_data);
    }

    /* Fuzz curl_multi_setopt with timer callback */
    if (Size >= sizeof(void *)) {
        void *timer_cb_data = (void *)Data;
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, timer_cb_data);
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}