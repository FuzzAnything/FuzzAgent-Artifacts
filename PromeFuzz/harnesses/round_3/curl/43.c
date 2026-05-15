// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
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
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"
#include <unistd.h>
#include <fcntl.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        } else {
            const char *default_data = "dummy data for fuzzing";
            fwrite(default_data, 1, strlen(default_data), fp);
        }
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
    // Create dummy file for potential transfers
    create_dummy_file(data, size);
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Initialize easy handle for testing
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set some options to make the handle valid
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // Test curl_multi_timeout
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);
    
    // Test curl_multi_waitfds (if available)
    unsigned int fd_count = 0;
    struct curl_waitfd *waitfds = NULL;
    
    // First call to get count
    curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    if (fd_count > 0) {
        waitfds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
        if (waitfds) {
            // Second call to get actual fds
            curl_multi_waitfds(multi_handle, waitfds, fd_count, &fd_count);
            free(waitfds);
        }
    }
    
    // Test curl_multi_wait
    int ret = 0;
    struct curl_waitfd extra_fds[2];
    
    // Set up some dummy file descriptors
    extra_fds[0].fd = 0;  // stdin
    extra_fds[0].events = (short)(data[0] % 3);  // Random events
    extra_fds[1].fd = 1;  // stdout
    extra_fds[1].events = (short)((size > 1 ? data[1] : 0) % 3);
    
    int timeout = (size > 2 ? (int)(data[2] % 1000) : 100);  // Random timeout up to 1000ms
    curl_multi_wait(multi_handle, extra_fds, 2, timeout, &ret);
    
    // Test curl_multi_perform
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Test curl_multi_get_offt for different info types
    curl_off_t value = 0;
    
    // Try different CURLMinfo_offt values
    for (int i = 0; i < 4; i++) {
        curl_multi_get_offt(multi_handle, (CURLMinfo_offt)i, &value);
    }
    
    // Test curl_multi_remove_handle if we added one
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    // Cleanup
    curl_multi_cleanup(multi_handle);
    cleanup_dummy_file();
    
    return 0;
}