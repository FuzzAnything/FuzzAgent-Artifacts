// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy; (void)s; (void)what; (void)userp; (void)socketp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) return 0;

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }

    /* Prepare dummy file for options that might require it */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Initialize error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, CURL_ERROR_SIZE);

    struct curl_slist *headers = NULL;

    /* Test curl_easy_setopt with various options derived from input */
    if (Size > 0) {
        /* Set URL if input looks like a string (basic check) */
        if (Size < 256) {
            char url[257];
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        }

        /* Set callbacks */
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
        curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buffer);

        /* Test type checking macros implicitly by passing correct types */
        /* Wcurl_easy_setopt_err_seek_cb is a macro, not a function, so we use the real opt */
        curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
        
        /* Test Wcurl_easy_setopt_err_error_buffer context */
        curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, error_buffer);

        /* Test Wcurl_easy_setopt_err_curl_slist context */
        headers = curl_slist_append(headers, "Test: Header");
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);

        /* Add to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* Test curl_multi_info_read */
    int msgs_left;
    CURLMsg *msg = NULL;
    while ((msg = curl_multi_info_read(multi_handle, &msgs_left)) != NULL) {
        (void)msg; /* Use the message to avoid unused variable warning */
    }

    /* Test curl_mprintf */
    if (Size > 0 && Size < 100) {
        /* Ensure format string is safe by limiting to simple chars or using a fixed format */
        curl_mprintf("Fuzz input size: %zu\n", Size);
    }

    /* Cleanup */
    if (headers) {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}