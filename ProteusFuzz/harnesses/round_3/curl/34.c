// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <sys/socket.h>
#include <unistd.h>

static void cleanup_multi(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    unsigned int notification = 0;
    if (Size > 0) {
        notification = Data[0];
    }
    if (Size > 1) {
        notification |= ((unsigned int)Data[1] << 8);
    }
    if (Size > 2) {
        notification |= ((unsigned int)Data[2] << 16);
    }
    if (Size > 3) {
        notification |= ((unsigned int)Data[3] << 24);
    }

    // Test curl_multi_notify_enable
    curl_multi_notify_enable(multi, notification);

    // Test curl_multi_notify_disable
    curl_multi_notify_disable(multi, notification);

    // Test curl_multi_strerror with various codes
    for (size_t i = 0; i < 10 && i < Size; i++) {
        CURLMcode code = (CURLMcode)(Data[i]);
        const char *str = curl_multi_strerror(code);
        (void)str; // Use the string to avoid unused variable warning
    }

    // Test curl_multi_waitfds
    struct curl_waitfd *ufds = NULL;
    unsigned int fd_count = 0;
    
    // First call to get count
    curl_multi_waitfds(multi, NULL, 0, &fd_count);
    
    if (fd_count > 0) {
        ufds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            memset(ufds, 0, fd_count * sizeof(struct curl_waitfd));
            curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
        }
    }

    // Test curl_multi_wait
    int ret = 0;
    int timeout_ms = 0;
    if (Size > 4) {
        timeout_ms = (int)(Data[4] % 1000);
    }
    
    curl_multi_wait(multi, ufds, fd_count, timeout_ms, &ret);

    // Cleanup
    if (ufds) {
        free(ufds);
    }
    cleanup_multi(multi);

    return 0;
}