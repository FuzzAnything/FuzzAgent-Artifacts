// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl_multi_setopt, curl_multi_strerror, curl_multi_wakeup,
 * and curl_multi_add_handle.
 */

static CURLM *g_multi_handle = NULL;
static CURL *g_easy_handle = NULL;

/*
 * Simple push callback for CURLMOPT_PUSHFUNCTION.
 * Returns CURL_PUSH_OK to accept, CURL_PUSH_DENY to deny.
 */
static int push_callback(CURL *parent, CURL *easy, size_t num_headers,
                         struct curl_header *headers, void *userp) {
    (void)parent;
    (void)easy;
    (void)num_headers;
    (void)headers;
    (void)userp;
    return CURL_PUSH_OK;
}

/*
 * Initialize curl global state and create handles.
 */
static void init_curl_handles(void) {
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return;
    }

    g_multi_handle = curl_multi_init();
    if (!g_multi_handle) {
        curl_global_cleanup();
        return;
    }

    g_easy_handle = curl_easy_init();
    if (!g_easy_handle) {
        curl_multi_cleanup(g_multi_handle);
        g_multi_handle = NULL;
        curl_global_cleanup();
        return;
    }
}

/*
 * Cleanup curl global state and handles.
 */
static void cleanup_curl_handles(void) {
    if (g_easy_handle) {
        curl_easy_cleanup(g_easy_handle);
        g_easy_handle = NULL;
    }

    if (g_multi_handle) {
        curl_multi_cleanup(g_multi_handle);
        g_multi_handle = NULL;
    }

    curl_global_cleanup();
}

/*
 * Parse fuzz input to derive options and values.
 */
static CURLMoption get_multi_option_from_input(const uint8_t *data, size_t size) {
    if (size == 0) return CURLMOPT_MAXCONNECTS;

    uint8_t opt_idx = data[0] % 10;

    switch (opt_idx) {
        case 0: return CURLMOPT_MAXCONNECTS;
        case 1: return CURLMOPT_MAX_HOST_CONNECTIONS;
        case 2: return CURLMOPT_PIPELINING;
        case 3: return CURLMOPT_PUSHFUNCTION;
        case 4: return CURLMOPT_PUSHDATA;
        case 5: return CURLMOPT_SOCKETFUNCTION;
        case 6: return CURLMOPT_SOCKETDATA;
        case 7: return CURLMOPT_TIMERFUNCTION;
        case 8: return CURLMOPT_TIMERDATA;
        case 9: return CURLMOPT_MAXCONNECTS;
        default: return CURLMOPT_MAXCONNECTS;
    }
}

static long get_long_value_from_input(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    long val = 0;
    memcpy(&val, data, 4);
    if (val < -1000000) val = -1000000;
    if (val > 1000000) val = 1000000;
    return val;
}

static void *get_ptr_value_from_input(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    return (void *)data;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    if (!g_multi_handle || !g_easy_handle) {
        init_curl_handles();
        if (!g_multi_handle || !g_easy_handle) {
            return 0;
        }
    }

    CURLMoption option = get_multi_option_from_input(Data, Size);
    CURLMcode res;

    switch (option) {
        case CURLMOPT_MAXCONNECTS:
        case CURLMOPT_MAX_HOST_CONNECTIONS:
        case CURLMOPT_PIPELINING: {
            long val = get_long_value_from_input(Data, Size);
            res = curl_multi_setopt(g_multi_handle, option, val);
            (void)res;
            break;
        }
        case CURLMOPT_PUSHFUNCTION: {
            res = curl_multi_setopt(g_multi_handle, option, push_callback);
            (void)res;
            break;
        }
        case CURLMOPT_PUSHDATA: {
            void *ptr = get_ptr_value_from_input(Data, Size);
            res = curl_multi_setopt(g_multi_handle, option, ptr);
            (void)res;
            break;
        }
        case CURLMOPT_SOCKETFUNCTION:
        case CURLMOPT_TIMERFUNCTION: {
            res = curl_multi_setopt(g_multi_handle, option, NULL);
            (void)res;
            break;
        }
        case CURLMOPT_SOCKETDATA:
        case CURLMOPT_TIMERDATA: {
            void *ptr = get_ptr_value_from_input(Data, Size);
            res = curl_multi_setopt(g_multi_handle, option, ptr);
            (void)res;
            break;
        }
        default:
            long val = get_long_value_from_input(Data, Size);
            res = curl_multi_setopt(g_multi_handle, option, val);
            (void)res;
            break;
    }

    /* Fixed: Check size before reading 2 bytes for error_code */
    if (Size >= 2) {
        int16_t error_code;
        memcpy(&error_code, Data, sizeof(int16_t));
        const char *str = curl_multi_strerror((CURLMcode)error_code);
        (void)str;
    }

    res = curl_multi_wakeup(g_multi_handle);
    (void)res;

    res = curl_multi_add_handle(g_multi_handle, g_easy_handle);
    (void)res;

    res = curl_multi_add_handle(g_multi_handle, g_easy_handle);
    (void)res;

    res = curl_multi_add_handle(g_multi_handle, NULL);
    (void)res;

    res = curl_multi_add_handle(NULL, g_easy_handle);
    (void)res;

    cleanup_curl_handles();

    return 0;
}