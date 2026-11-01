// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
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
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Dummy file for file-based operations */
#define DUMMY_FILE "./dummy_file"

/* Callback function for curl_formget */
static size_t formget_callback(void *arg, const char *ptr, size_t size) {
    (void)arg;
    (void)ptr;
    (void)size;
    return size;
}

/* Dummy read callback for curl_easy_setopt testing */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Dummy write callback for curl_easy_setopt testing */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Dummy conversion callback for curl_easy_setopt testing */
static char *dummy_conv_cb(char *ptr, size_t size, void *userdata) {
    (void)size;
    (void)userdata;
    return ptr;
}

/* Dummy seek callback for curl_easy_setopt testing */
static int dummy_seek_cb(void *stream, curl_off_t offset, int origin) {
    (void)stream;
    (void)offset;
    (void)origin;
    return 0;
}

/* Dummy socket callback for curl_multi_setopt testing */
static int dummy_socket_cb(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

/* Dummy timer callback for curl_multi_setopt testing */
static int dummy_timer_cb(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Create dummy file if needed */
    FILE *f = fopen(DUMMY_FILE, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Test curl_formadd */
    struct curl_httppost *post = NULL;
    struct curl_httppost *last_post = NULL;
    char *name = (char *)malloc(Size + 1);
    char *value = (char *)malloc(Size + 1);
    if (name && value) {
        memcpy(name, Data, Size);
        name[Size] = '\0';
        memcpy(value, Data, Size);
        value[Size] = '\0';

        CURLFORMcode res = curl_formadd(&post, &last_post,
            CURLFORM_COPYNAME, name,
            CURLFORM_COPYCONTENTS, value,
            CURLFORM_END);
        (void)res;

        /* Test curl_formget */
        if (post) {
            int res2 = curl_formget(post, NULL, formget_callback);
            (void)res2;
        }

        /* Test curl_formfree */
        if (post) {
            curl_formfree(post);
            post = NULL;
        }

        free(name);
        free(value);
    }

    /* Test curl_easy_setopt with various options */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

    /* Test curl_multi_setopt */
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, dummy_socket_cb);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, dummy_timer_cb);

    /* Test curl_mprintf */
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Fuzzing with size %zu", Size);
    int res = curl_mprintf("%s", buffer);
    (void)res;

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_multi_cleanup(multi);

    /* Remove dummy file */
    remove(DUMMY_FILE);

    return 0;
}