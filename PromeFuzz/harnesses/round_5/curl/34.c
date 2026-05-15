// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Define CURL_DISABLE_TYPECHECK to avoid typechecking macro issues */
#define CURL_DISABLE_TYPECHECK
#include "curl/curl.h"
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static CURL *create_easy_handle(const uint8_t *data, size_t size) {
    CURL *easy = curl_easy_init();
    if (easy) {
        write_dummy_file(data, size);
        curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, 1L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_TIME, 1L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    CURLM *multi = curl_multi_init();
    if (!multi) return 0;
    
    CURL *easy = create_easy_handle(data, size);
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }
    
    CURLMcode res;
    
    res = curl_multi_add_handle(multi, easy);
    if (res != CURLM_OK) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        return 0;
    }
    
    CURL **handles = curl_multi_get_handles(multi);
    if (handles) {
        free(handles);
    }
    
    unsigned int fd_count = 0;
    res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
    if (res == CURLM_OK && fd_count > 0) {
        struct curl_waitfd *ufds = (struct curl_waitfd*)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            res = curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }
    
    int numfds = 0;
    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = 0;
    extra_fds[0].events = 0x01;  /* CURL_WAIT_POLLIN */
    extra_fds[0].revents = 0;
    
    extra_fds[1].fd = 1;
    extra_fds[1].events = 0x04;  /* CURL_WAIT_POLLOUT */
    extra_fds[1].revents = 0;
    
    int timeout = (int)(data[0] % 100);
    res = curl_multi_wait(multi, extra_fds, 2, timeout, &numfds);
    
    res = curl_multi_remove_handle(multi, easy);
    
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    
    unlink("./dummy_file");
    return 0;
}