// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "curl/curl.h"
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    write_dummy_file(Data, Size);

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;

    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    long timeout = 0;
    curl_multi_timeout(multi_handle, &timeout);

    unsigned int fd_count = 0;
    struct curl_waitfd *ufds = NULL;
    curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    if (fd_count > 0 && fd_count < 100) {
        ufds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }

    int extra_nfds = (Size > 0) ? (Data[0] % 4) : 0;
    struct curl_waitfd extra_fds[4];
    for (int i = 0; i < extra_nfds && i < 4; i++) {
        extra_fds[i].fd = open("/dev/null", O_RDONLY);
        extra_fds[i].events = (Data[i % Size] & 1) ? CURL_WAIT_POLLIN : CURL_WAIT_POLLOUT;
        extra_fds[i].revents = 0;
    }

    int ret = 0;
    int timeout_ms = (timeout > 0 && timeout < 1000) ? (int)timeout : 100;
    curl_multi_wait(multi_handle, extra_nfds > 0 ? extra_fds : NULL,
                    (unsigned int)extra_nfds, timeout_ms, &ret);

    for (int i = 0; i < extra_nfds && i < 4; i++) {
        if (extra_fds[i].fd >= 0) close(extra_fds[i].fd);
    }

    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    curl_off_t value = 0;
    for (int i = 0; i < 4; i++) {
        curl_multi_get_offt(multi_handle, (CURLMinfo_offt)i, &value);
    }

    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    curl_multi_cleanup(multi_handle);
    remove_dummy_file();
    return 0;
}