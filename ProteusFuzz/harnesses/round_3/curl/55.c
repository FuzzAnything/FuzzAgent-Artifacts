// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_duphandle at easy.c:952:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
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
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr; (void)size; (void)nmemb; (void)userptr;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr; (void)size; (void)nmemb; (void)userptr;
    return size * nmemb;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_hstsread_callback(void *userptr, const char *host, char *buf, size_t buflen) {
    (void)userptr; (void)host; (void)buf; (void)buflen;
    return 0;
}

/* Correct signature for curl_ssls_export_cb based on curl.h */
static CURLcode dummy_ssls_export_callback(
    void *userptr,
    void *session,
    const char *session_id,
    const unsigned char *session_data,
    size_t session_data_len,
    const unsigned char *session_key,
    size_t session_key_len,
    long session_age,
    int session_version,
    const char *session_cipher,
    size_t session_cipher_len) {
    (void)userptr; (void)session; (void)session_id; (void)session_data; (void)session_data_len;
    (void)session_key; (void)session_key_len; (void)session_age; (void)session_version;
    (void)session_cipher; (void)session_cipher_len;
    return CURLE_OK;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, curlsocktype purpose, void *userptr, void *sockptr) {
    (void)easy; (void)s; (void)purpose; (void)userptr; (void)sockptr;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userptr) {
    (void)multi; (void)timeout_ms; (void)userptr;
    return 0;
}

/* Correct signature for curl_push_callback */
static int dummy_push_callback(CURL *easy, const struct curl_pushheaders_t *headers, void *userptr) {
    (void)easy; (void)headers; (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a dummy file if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzzing logic */
    unsigned int action = Data[0] % 10;

    switch (action) {
        case 0:
            /* Test curl_easy_duphandle */
            {
                CURL *dup = curl_easy_duphandle(curl);
                if (dup) {
                    /* Perform some operations on the duplicate */
                    curl_easy_setopt(dup, CURLOPT_URL, "http://example.com");
                    curl_easy_setopt(dup, CURLOPT_WRITEFUNCTION, dummy_write_callback);
                    curl_easy_setopt(dup, CURLOPT_WRITEDATA, (void*)Data);
                    curl_easy_cleanup(dup);
                }
            }
            break;

        case 1:
            /* Test curl_easy_ssls_export */
            {
                curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPN, 1L);
                curl_easy_ssls_export(curl, dummy_ssls_export_callback, (void*)Data);
            }
            break;

        case 2:
            /* Test curl_easy_setopt with string argument */
            {
                if (Size > 1) {
                    char *str = (char*)malloc(Size);
                    if (str) {
                        memcpy(str, Data, Size);
                        str[Size - 1] = '\0';
                        curl_easy_setopt(curl, CURLOPT_URL, str);
                        free(str);
                    }
                }
            }
            break;

        case 3:
            /* Test curl_easy_setopt with callback argument */
            {
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
                curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
            }
            break;

        case 4:
            /* Test curl_easy_setopt with HSTS callback */
            {
                curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
                curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void*)Data);
            }
            break;

        case 5:
            /* Test curl_multi_setopt with timer callback */
            {
                curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                curl_multi_setopt(multi, CURLMOPT_TIMERDATA, (void*)Data);
            }
            break;

        case 6:
            /* Test curl_multi_setopt with socket callback */
            {
                curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, (void*)Data);
            }
            break;

        case 7:
            /* Test curl_multi_setopt with push callback */
            {
                curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
                curl_multi_setopt(multi, CURLMOPT_PUSHDATA, (void*)Data);
            }
            break;

        case 8:
            /* Test curl_multi_setopt with char** argument (e.g., CURLMOPT_PUSHHEADER is not valid, use CURLMOPT_PUSHFUNCTION instead or other valid options) */
            /* Since CURLMOPT_PUSHHEADER doesn't exist, we skip this specific test or use a valid one like CURLMOPT_MAXCONNECTS */
            {
                long val = 10;
                if (Size > 1) {
                    val = (long)(Data[1] % 100);
                }
                curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, val);
            }
            break;

        case 9:
            /* Test various setopt combinations */
            {
                curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)Data);
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
                curl_easy_setopt(curl, CURLOPT_READDATA, (void*)Data);
                curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
                curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)Data);
            }
            break;
    }

    /* Cleanup */
    curl_multi_cleanup(multi);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}