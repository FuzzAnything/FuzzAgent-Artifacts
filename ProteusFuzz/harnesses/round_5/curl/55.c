// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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

/* Dummy callback definitions to satisfy type checking macros if needed */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_ssh_hostkey_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return 0;
}

static int dummy_push_cb(CURL *curl, const char *url, void *userdata) {
    (void)curl; (void)url; (void)userdata;
    return 0;
}

static int dummy_socket_cb(CURLM *multi, curl_socket_t sockfd, int what, void *userp, void *socketp) {
    (void)multi; (void)sockfd; (void)what; (void)userp; (void)socketp;
    return 0;
}

static int dummy_timer_cb(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

/* Helper to safely create a dummy file */
static FILE* create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f && Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    if (f) {
        fclose(f);
        f = fopen("./dummy_file", "rb");
    }
    return f;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    CURL **handles = NULL;
    FILE *file_ptr = NULL;
    int result = 0;
    long long_val = 0;
    curl_off_t off_val = 0;
    char *str_ptr = NULL;
    struct curl_slist *slist = NULL;
    char error_buf[CURL_ERROR_SIZE];

    /* Initialize curl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Prepare dummy data */
    if (Size > 0) {
        str_ptr = (char *)malloc(Size + 1);
        if (str_ptr) {
            memcpy(str_ptr, Data, Size);
            str_ptr[Size] = '\0';
        }
    }

    file_ptr = create_dummy_file(Data, Size);

    /* Fuzzing curl_easy_setopt with various types to trigger Wcurl_easy_setopt_err_* warnings/macros */
    
    /* Long option */
    if (Size > 0) long_val = (long)(Data[0]);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, long_val);

    /* Off_t option */
    if (Size > 1) off_val = (curl_off_t)((Data[0] << 8) | Data[1]);
    curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, off_val);

    /* String option */
    if (str_ptr) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, str_ptr);
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, str_ptr);
    }

    /* FILE option */
    if (file_ptr) {
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, file_ptr);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, file_ptr);
    }

    /* Postfields option */
    if (str_ptr) {
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, str_ptr);
    }

    /* Slist option */
    if (str_ptr) {
        slist = curl_slist_append(slist, str_ptr);
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, slist);
    }

    /* Callback options */
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);

    /* Error buffer */
    curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buf);

    /* Fuzzing curl_multi_setopt with various types to trigger Wcurl_multi_setopt_err_* warnings/macros */
    
    /* char** option (MIMEPOST is not valid for multi, but we test type checking logic) */
    /* Note: CURLMOPT_PUSHFUNCTION expects a callback, not char** */
    /* Testing valid multi options */
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_cb);
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_cb);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_cb);

    /* Fuzzing curl_multi_get_handles */
    handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        CURL **h = handles;
        while (*h) {
            h++;
        }
        curl_free(handles);
    }

    /* Cleanup */
    if (slist) {
        curl_slist_free_all(slist);
    }
    if (file_ptr) {
        fclose(file_ptr);
    }
    if (str_ptr) {
        free(str_ptr);
    }
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}