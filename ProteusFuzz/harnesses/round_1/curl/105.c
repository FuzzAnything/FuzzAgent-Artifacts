// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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

/* Dummy callback implementations to satisfy type checks if needed */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_progress_callback(CURL *handle, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)handle; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Use fuzzing data to set various options */
    /* Set URL from data if it looks like a URL (basic check) */
    if (Size > 10) {
        char url_buf[256];
        size_t url_len = (Size < 255) ? Size : 255;
        /* Ensure we don't read past the buffer */
        if (url_len > Size) url_len = Size;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
    }

    /* Set dummy callbacks to exercise type checking macros */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);

    /* Set some string options using fuzzing data */
    if (Size > 20) {
        char user_agent[128];
        size_t ua_len = (Size - 20 < 127) ? (Size - 20) : 127;
        /* Safety check to ensure we don't read past the buffer */
        if (ua_len > Size - 20) ua_len = Size - 20;
        memcpy(user_agent, Data + 20, ua_len);
        user_agent[ua_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, user_agent);
    }

    /* Set long options using fuzzing data */
    if (Size > 1) {
        long timeout = (long)Data[0];
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
    }

    /* Exercise curl_multi_socket with fuzzing data */
    if (Size > 2) {
        curl_socket_t socket_fd = (curl_socket_t)((Data[0] << 8) | Data[1]);
        int running_handles = 0;
        CURLMcode res = curl_multi_socket(multi_handle, socket_fd, &running_handles);
        (void)res; /* Ignore return value for fuzzing */
    }

    /* Exercise curl_multi_socket_all with fuzzing data */
    if (Size > 0) {
        int running_handles = 0;
        CURLMcode res = curl_multi_socket_all(multi_handle, &running_handles);
        (void)res; /* Ignore return value for fuzzing */
    }

    /* Exercise curl_easy_getinfo with various info types */
    long long_val = 0;
    double double_val = 0.0;
    char *string_val = NULL;
    struct curl_tlssessioninfo *tls_session = NULL;
    struct curl_certinfo *cert_info = NULL;
    curl_socket_t socket_val = 0;
    curl_off_t off_val = 0;

    /* Try to get various info types */
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &double_val);
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &string_val);
    curl_easy_getinfo(easy_handle, CURLINFO_TLS_SESSION, &tls_session);
    curl_easy_getinfo(easy_handle, CURLINFO_CERTINFO, &cert_info);
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &socket_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_UPLOAD_T, &off_val);

    /* Cleanup string info if allocated */
    if (string_val) {
        /* Note: curl_easy_getinfo does not allocate memory for string_val in this context,
           but in real usage, it might. For safety, we don't free it here. */
    }

    /* Remove easy handle from multi handle */
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}