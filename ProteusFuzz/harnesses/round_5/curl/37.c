// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_unescape at escape.c:163:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback functions to satisfy type checks */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle; (void)type; (void)data; (void)size; (void)userdata;
    return 0;
}

static int dummy_progress_cb(CURL *handle, double dltotal, double dlnow, double ultotal, double ulnow, void *userdata) {
    (void)handle; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow; (void)userdata;
    return 0;
}

/* Helper to create a dummy file */
static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Initialize easy handle and multi handle */
    CURL *easy_handle = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();

    if (!easy_handle || !multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Create a dummy file for testing file-based options if needed */
    create_dummy_file("./dummy_file", Data, Size);

    /* Fuzz curl_mprintf */
    /* Ensure Data is null-terminated for format string safety */
    char *safe_data = malloc(Size + 1);
    if (safe_data) {
        memcpy(safe_data, Data, Size);
        safe_data[Size] = '\0';
        /* Use a safe format to avoid format string vulnerabilities */
        curl_mprintf("%s", safe_data);
        free(safe_data);
    }

    /* Fuzz curl_msprintf */
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (Size > 0) {
        /* Ensure the string passed is null-terminated to prevent heap-buffer-overflow in strlen */
        char *safe_str = malloc(Size + 1);
        if (safe_str) {
            memcpy(safe_str, Data, Size);
            safe_str[Size] = '\0';
            /* Use a safe format */
            curl_msprintf(buffer, "%s", safe_str);
            free(safe_str);
        }
    }

    /* Fuzz curl_easy_unescape */
    char *unescaped = NULL;
    int outlength = 0;
    if (Size > 0) {
        unescaped = curl_easy_unescape(easy_handle, (char*)Data, (int)Size, &outlength);
        if (unescaped) {
            free(unescaped);
        }
    }

    /* Fuzz curl_strequal */
    if (Size >= 2) {
        char s1[256], s2[256];
        size_t len = (Size > 255) ? 255 : Size;
        memcpy(s1, Data, len);
        s1[len] = '\0';
        memcpy(s2, Data, len);
        s2[len] = '\0';
        curl_strequal(s1, s2);
        curl_strequal(NULL, NULL);
        curl_strequal(s1, NULL);
    }

    /* Fuzz curl_multi_info_read */
    int msgs_in_queue = 0;
    CURLMsg *msg = NULL;
    /* Try to read messages (likely empty queue) */
    msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    (void)msg; /* Suppress unused variable warning */

    /* Fuzz curl_easy_setopt with various options to trigger type checking */
    
    /* String options: Ensure null-termination to prevent buffer overflows in curl */
    char *url_str = malloc(Size + 1);
    if (url_str) {
        memcpy(url_str, Data, Size);
        url_str[Size] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url_str);
        free(url_str);
    }

    char *agent_str = malloc(Size + 1);
    if (agent_str) {
        memcpy(agent_str, Data, Size);
        agent_str[Size] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, agent_str);
        free(agent_str);
    }
    
    /* Long options */
    long long_val = (Size > 0) ? (long)Data[0] : 0L;
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, long_val);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, long_val);

    /* Callback options */
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    
    /* Callback data options */
    void *cb_data = (void*)(uintptr_t)Data;
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, cb_data);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, cb_data);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGDATA, cb_data);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, cb_data);

    /* File options */
    curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE, "./dummy_file");
    curl_easy_setopt(easy_handle, CURLOPT_CAINFO, "./dummy_file");

    /* Multi handle options */
    /* Note: Wcurl_multi_setopt_err_cb_data is a compile-time warning macro, 
       not a runtime function. We use curl_multi_setopt with correct types. */
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, cb_data);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}