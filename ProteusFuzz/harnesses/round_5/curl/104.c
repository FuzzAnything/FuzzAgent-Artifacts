// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)userdata;
    return nmemb * size;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Create a dummy file for FILE* options
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, (const char *)Data, Size);
        close(fd);
    }
    FILE *fp = fopen("./dummy_file", "r");

    // Set various options to explore different states
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, (void *)Data);

    // Try setting a FILE* option if fp is valid
    if (fp) {
        curl_easy_setopt(easy, CURLOPT_INFILESIZE_LARGE, (curl_off_t)Size);
        // Note: We don't actually set CURLOPT_INFILE here to avoid closing the file
        // which might be used by the fuzzer or cause issues.
        // Instead, we rely on the read callback.
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_TIMEOUT;
    int ev_bitmask = 0;

    // Fuzz curl_multi_socket_action
    if (Size > 0) {
        s = (curl_socket_t)Data[0];
        ev_bitmask = (Size > 1) ? Data[1] : 0;
    }
    curl_multi_socket_action(multi, s, ev_bitmask, &running_handles);

    // Fuzz curl_multi_socket (deprecated)
    curl_multi_socket(multi, s, &running_handles);

    // Fuzz curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi, &running_handles);

    // Fuzz curl_multi_assign
    void *sockp = (void *)(size_t)Data[0];
    curl_multi_assign(multi, s, sockp);

    // Fuzz curl_multi_perform
    curl_multi_perform(multi, &running_handles);

    // Cleanup
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    if (fp) {
        fclose(fp);
    }
    unlink("./dummy_file");

    return 0;
}