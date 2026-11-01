// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void dummy_file_setup(void) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        const char *data = "dummy content for fuzzing";
        write(fd, data, strlen(data));
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Setup dummy file for potential file-based operations if needed */
    dummy_file_setup();

    /* Fuzz curl_multi_remove_handle */
    (void)curl_multi_remove_handle(multi_handle, easy_handle);
    (void)curl_multi_remove_handle(multi_handle, NULL);

    /* Fuzz curl_multi_strerror with various inputs derived from fuzz data */
    for (size_t i = 0; i < Size; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *str = curl_multi_strerror(code);
        (void)str; /* Suppress unused variable warning */
    }

    /* Fuzz curl_multi_assign */
    int dummy_socket = -1;
    void *custom_data = (void *)(uintptr_t)Data[0];
    (void)curl_multi_assign(multi_handle, dummy_socket, custom_data);
    
    /* Check bounds before accessing Data[1] and Data[2] */
    if (Size >= 3) {
        (void)curl_multi_assign(multi_handle, (curl_socket_t)Data[1], (void *)(uintptr_t)Data[2]);
    }
    
    (void)curl_multi_assign(NULL, dummy_socket, custom_data);

    /* Fuzz curl_multi_waitfds */
    struct curl_waitfd ufds[10];
    unsigned int fd_count = 0;
    unsigned int size = (Size > 10) ? 10 : Size;
    memset(ufds, 0, sizeof(ufds));
    (void)curl_multi_waitfds(multi_handle, ufds, size, &fd_count);
    (void)curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    (void)curl_multi_waitfds(NULL, ufds, size, &fd_count);

    /* Fuzz curl_multi_wait */
    int ret = 0;
    int timeout_ms = (int)(Data[Size - 1] % 1000);
    struct curl_waitfd extra_fds[5];
    memset(extra_fds, 0, sizeof(extra_fds));
    unsigned int extra_nfds = (Size > 5) ? 5 : Size;
    (void)curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    (void)curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret);
    (void)curl_multi_wait(NULL, extra_fds, extra_nfds, timeout_ms, &ret);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}