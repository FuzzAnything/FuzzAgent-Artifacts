// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userp;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userp;
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
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

    // Initialize dummy file for file-based options if needed
    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size > 1024 ? 1024 : Size);
        close(fd);
    }

    // Fuzz curl_multi_setopt_err_notifycb related logic via type checking
    // Since these are macros, we trigger them via curl_multi_setopt with various callbacks
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
    curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, (void*)Data);
    curl_multi_setopt(multi, CURLMOPT_TIMERDATA, (void*)Data);

    // Fuzz curl_easy_setopt with various options to trigger type checking macros
    curl_easy_setopt(easy, CURLOPT_URL, (Size > 10 && Data[0] != 0) ? (char*)Data : "http://example.com");
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, (void*)Data);
    curl_easy_setopt(easy, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(easy, CURLOPT_DEBUGDATA, (void*)Data);
    curl_easy_setopt(easy, CURLOPT_FILE, NULL); // Test FILE* option

    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_code = (CURLMcode)(Data[0] & 0xFF);
    const char *str = curl_multi_strerror(error_code);
    (void)str; // Suppress unused warning

    // Fuzz curl_multi_assign
    curl_socket_t sockfd = (curl_socket_t)(Data[1] & 0xFF);
    void *sockp = (void*)(Data + 2);
    curl_multi_assign(multi, sockfd, sockp);

    // Fuzz curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = (int)(Data[3] & 0xFF);
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    curl_multi_fdset(multi, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi, easy);

    // Cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    // Remove dummy file
    if (fd >= 0) {
        unlink(dummy_file);
    }

    return 0;
}