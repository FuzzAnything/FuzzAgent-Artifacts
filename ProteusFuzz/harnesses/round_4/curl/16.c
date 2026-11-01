// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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
#include <unistd.h>
#include <sys/socket.h>

/* Dummy callback to satisfy setopt requirements if needed */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

/* Helper to create a valid easy handle with minimal setup */
static CURL *create_easy_handle(const uint8_t *data, size_t size) {
    CURL *handle = curl_easy_init();
    if (!handle) return NULL;

    /* Set a dummy URL to avoid immediate errors during perform, 
       though it will likely fail to connect, which is fine for fuzzing */
    if (size > 0) {
        /* Use input data as part of the URL if it looks safe, otherwise default */
        char url[256];
        size_t len = (size < sizeof(url) - 10) ? size : sizeof(url) - 10;
        memcpy(url, data, len);
        url[len] = '\0';
        
        /* Ensure it starts with http or https to be a valid URL string for curl */
        if (strncmp(url, "http", 4) != 0 && strncmp(url, "https", 5) != 0) {
            /* Fallback to a safe dummy URL if input is not a valid URL prefix */
            curl_easy_setopt(handle, CURLOPT_URL, "http://127.0.0.1:1");
        } else {
            curl_easy_setopt(handle, CURLOPT_URL, url);
        }
    } else {
        curl_easy_setopt(handle, CURLOPT_URL, "http://127.0.0.1:1");
    }

    /* Set timeouts to avoid hanging */
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 1L);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    
    /* Set dummy callbacks to prevent crashes if libcurl tries to write/debug */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(handle, CURLOPT_DEBUGDATA, NULL);

    return handle;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURL **handles = NULL;
    int running_handles = 0;
    CURLMcode rc;
    curl_socket_t sock;
    int ev_bitmask;

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle using input data */
    easy_handle = create_easy_handle(Data, Size);
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

    /* 
       Fuzzing Logic: 
       We invoke the target functions with various states derived from the input.
       We ensure cleanup is performed after each test case or at the end.
    */

    /* 1. Test curl_multi_perform */
    running_handles = 0;
    rc = curl_multi_perform(multi_handle, &running_handles);
    (void)rc; /* Ignore return for fuzzing stability, just trigger execution */

    /* 2. Test curl_multi_socket_all (Deprecated) */
    running_handles = 0;
    rc = curl_multi_socket_all(multi_handle, &running_handles);
    (void)rc;

    /* 3. Test curl_multi_socket (Deprecated) */
    /* Use input data to derive socket and bitmask if possible, else use defaults */
    sock = -1;
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&sock, Data, sizeof(curl_socket_t));
    }
    /* curl_multi_socket expects a valid socket or -1 to process all */
    rc = curl_multi_socket(multi_handle, sock, &running_handles);
    (void)rc;

    /* 4. Test curl_multi_socket_action */
    ev_bitmask = 0;
    /* Fix: Check size before reading ev_bitmask to prevent buffer overflow */
    if (Size >= (sizeof(curl_socket_t) + sizeof(int))) {
        memcpy(&ev_bitmask, Data + sizeof(curl_socket_t), sizeof(int));
    } else if (Size > sizeof(curl_socket_t)) {
        /* If we have some data but not enough for a full int, read what we can safely or default */
        /* For safety in fuzzing, we default to 0 if we can't read a full int */
        ev_bitmask = 0; 
    }
    /* Use the same socket logic */
    rc = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    (void)rc;

    /* 5. Test curl_multi_get_handles */
    handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        /* Inspect the array (no-op for fuzzing, just ensure no crash) */
        int i = 0;
        while (handles[i] != NULL) {
            i++;
        }
        curl_free(handles);
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}