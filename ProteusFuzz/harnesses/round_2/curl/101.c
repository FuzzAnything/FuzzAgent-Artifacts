// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions and type-checking macros.
 * Targets:
 * 1. curl_multi_socket_all
 * 2. curl_multi_perform
 * 3. curl_multi_socket
 *
 * Note: Wcurl_* macros are compile-time checks and cannot be invoked at runtime.
 * We test the underlying curl_easy_setopt logic by passing various types to trigger
 * potential runtime errors or logic paths related to type mismatches if the
 * library implementation has fallbacks or if the fuzzing environment disables
 * strict type checking.
 */

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

/* Helper to safely read a long from fuzz input */
static long read_long(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(long) > size) {
        return 0;
    }
    long val;
    memcpy(&val, data + *offset, sizeof(long));
    *offset += sizeof(long);
    return val;
}

/* Helper to safely read a socket handle from fuzz input */
static curl_socket_t read_socket(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(curl_socket_t) > size) {
        return CURL_SOCKET_BAD;
    }
    curl_socket_t val;
    memcpy(&val, data + *offset, sizeof(curl_socket_t));
    *offset += sizeof(curl_socket_t);
    return val;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a single easy handle to add to the multi handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to make the handle valid */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT, 1L);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi, easy);

    /* Initialize running_handles counter */
    int running_handles = 0;

    /* Use fuzz data to drive logic */
    size_t offset = 0;
    int action = Data[offset] % 10;

    /*
     * Fuzzing Strategy:
     * 1. Call curl_multi_perform
     * 2. Call curl_multi_socket_all
     * 3. Call curl_multi_socket with various socket values
     * 4. Attempt to set options with potentially mismatched types (runtime behavior)
     */

    switch (action) {
        case 0:
        case 1:
        case 2:
            /* Test curl_multi_perform */
            curl_multi_perform(multi, &running_handles);
            break;

        case 3:
        case 4:
        case 5:
            /* Test curl_multi_socket_all */
            curl_multi_socket_all(multi, &running_handles);
            break;

        case 6:
        case 7:
            /* Test curl_multi_socket with fuzzed socket value */
            if (offset + sizeof(curl_socket_t) <= Size) {
                curl_socket_t s = read_socket(Data, Size, &offset);
                curl_multi_socket(multi, s, &running_handles);
            }
            break;

        case 8:
            /* Test setting options with potential type mismatches */
            /* We use the macros implicitly by calling curl_easy_setopt with various types.
             * The macros are compile-time, but the runtime library might handle bad types.
             * We pass raw data pointers where longs are expected and vice versa to stress test.
             */
            if (offset + sizeof(long) <= Size) {
                long val = read_long(Data, Size, &offset);
                /* Try setting a long option with a pointer (if val is cast to pointer) */
                curl_easy_setopt(easy, CURLOPT_TIMEOUT, (void*)(uintptr_t)val);
            }
            /* Try setting a pointer option with a long */
            if (offset + sizeof(long) <= Size) {
                long val = read_long(Data, Size, &offset);
                curl_easy_setopt(easy, CURLOPT_URL, (char*)(uintptr_t)val);
            }
            break;

        case 9:
            /* Mix of operations */
            curl_multi_perform(multi, &running_handles);
            if (offset + sizeof(curl_socket_t) <= Size) {
                curl_socket_t s = read_socket(Data, Size, &offset);
                curl_multi_socket(multi, s, &running_handles);
            }
            curl_multi_socket_all(multi, &running_handles);
            break;

        default:
            /* Default to perform */
            curl_multi_perform(multi, &running_handles);
            break;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    return 0;
}