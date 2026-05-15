// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Define CURL_DISABLE_TYPECHECK to avoid the problematic macro expansion */
#define CURL_DISABLE_TYPECHECK

/* Include curl headers with the required path */
#include "curl/curl.h"
#include "curl/multi.h"

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, fp);
        } else {
            const char *default_data = "dummy data";
            fwrite(default_data, 1, strlen(default_data), fp);
        }
        fclose(fp);
    }
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    static int dummy_fd = -1;
    
    /* Initialize curl globally if needed */
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }
    
    /* Create dummy file with fuzzer data */
    create_dummy_file(Data, Size);
    
    /* Initialize easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        return 0;
    }
    
    /* Set up easy handle with dummy options */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);
    
    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }
    
    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);
    
    /* Use fuzzer data to determine which functions to call */
    uint8_t selector = Size > 0 ? Data[0] : 0;
    size_t data_offset = Size > 1 ? 1 : 0;
    
    /* Call different functions based on selector */
    switch (selector % 6) {
        case 0: {
            /* Test curl_multi_socket (deprecated but still available) */
            int running_handles = 0;
            curl_socket_t sock = (curl_socket_t)(dummy_fd);
            if (dummy_fd == -1) {
                dummy_fd = open("./dummy_file", O_RDONLY);
                sock = (curl_socket_t)(dummy_fd);
            }
            if (sock != (curl_socket_t)-1) {
                curl_multi_socket(multi_handle, sock, &running_handles);
            }
            break;
        }
        case 1: {
            /* Test curl_multi_remove_handle */
            curl_multi_remove_handle(multi_handle, easy_handle);
            /* Re-add it for subsequent operations */
            curl_multi_add_handle(multi_handle, easy_handle);
            break;
        }
        case 2: {
            /* Test curl_multi_wakeup */
            curl_multi_wakeup(multi_handle);
            break;
        }
        case 3: {
            /* Test curl_multi_wait */
            int ret = 0;
            struct curl_waitfd extra_fds[1];
            int timeout_ms = 10; /* Short timeout for fuzzing */
            
            /* Initialize extra_fds if we have enough data */
            if (Size - data_offset >= sizeof(int)) {
                int fd_value;
                memcpy(&fd_value, Data + data_offset, sizeof(int));
                extra_fds[0].fd = (curl_socket_t)(fd_value & 0xFFFF);
                extra_fds[0].events = CURL_WAIT_POLLIN;
                extra_fds[0].revents = 0;
                curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
            } else {
                curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret);
            }
            break;
        }
        case 4: {
            /* Test curl_multi_waitfds */
            unsigned int fd_count = 0;
            /* First call to get count */
            curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
            
            if (fd_count > 0) {
                struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    /* Second call to get actual fds */
                    curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
                    free(ufds);
                }
            }
            break;
        }
        case 5: {
            /* Test curl_multi_perform */
            int running_handles = 0;
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
    }
    
    /* Clean up */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    /* Close dummy fd if opened */
    if (dummy_fd != -1) {
        close(dummy_fd);
        dummy_fd = -1;
    }
    
    return 0;
}