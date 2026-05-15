// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Disable type checking macros to avoid compilation errors */
#define CURL_DISABLE_TYPECHECK
#include "multi.h"

/* Dummy callbacks for setopt */
static int dummy_notify_cb(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_socket_cb(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_cb(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    /* Initialize curl globally if needed */
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    /* Create dummy file if needed */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            const char dummy_data[] = "dummy";
            write(fd, dummy_data, sizeof(dummy_data) - 1);
        }
        close(fd);
    }

    /* Create handles */
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Add easy handle to multi handle for removal test */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Use fuzz data to derive parameters */
    unsigned int notification = 0;
    curl_socket_t socket = 0;
    int ev_bitmask = 0;
    int running_handles = 0;
    CURLMoption option = 0;

    if (Size >= sizeof(unsigned int)) {
        memcpy(&notification, Data, sizeof(unsigned int));
        if (Size >= 2 * sizeof(unsigned int)) {
            memcpy(&socket, Data + sizeof(unsigned int), sizeof(curl_socket_t));
            if (Size >= 2 * sizeof(unsigned int) + sizeof(int)) {
                memcpy(&ev_bitmask, Data + sizeof(unsigned int) + sizeof(curl_socket_t), sizeof(int));
                if (Size >= 2 * sizeof(unsigned int) + 2 * sizeof(int)) {
                    memcpy(&option, Data + sizeof(unsigned int) + sizeof(curl_socket_t) + sizeof(int), sizeof(CURLMoption));
                }
            }
        }
    }

    /* Test curl_multi_notify_enable */
    curl_multi_notify_enable(multi_handle, notification);

    /* Test curl_multi_setopt with various options */
    (curl_multi_setopt)(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_cb);
    (curl_multi_setopt)(multi_handle, CURLMOPT_NOTIFYDATA, NULL);
    (curl_multi_setopt)(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_cb);
    (curl_multi_setopt)(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_cb);
    
    /* Use fuzzer-derived option if valid */
    if (option >= 0) {
        (curl_multi_setopt)(multi_handle, option, NULL);
    }

    /* Test curl_multi_notify_disable */
    curl_multi_notify_disable(multi_handle, notification);

    /* Test curl_multi_socket_action with fuzzer-derived parameters */
    curl_multi_socket_action(multi_handle, socket, ev_bitmask, &running_handles);

    /* Test curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Test curl_multi_remove_handle */
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    /* Remove dummy file */
    unlink("./dummy_file");

    return 0;
}