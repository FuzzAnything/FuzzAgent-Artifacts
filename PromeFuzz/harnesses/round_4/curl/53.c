// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Include curl headers in the correct order */
#include "curl/curl.h"
#include "curl/multi.h"

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void dummy_file_cleanup(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    // Write dummy data to file for potential URL usage
    dummy_file_write(data, size);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        dummy_file_cleanup();
        return 0;
    }

    // Create easy handles
    CURL *easy_handles[3] = {NULL};
    int num_easy = (data[0] % 3) + 1; // 1-3 easy handles
    
    for (int i = 0; i < num_easy; i++) {
        easy_handles[i] = curl_easy_init();
        if (easy_handles[i]) {
            // Set some options to make handle valid
            curl_easy_setopt(easy_handles[i], CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(easy_handles[i], CURLOPT_NOPROGRESS, 1L);
            curl_multi_add_handle(multi_handle, easy_handles[i]);
        }
    }

    // Use data to influence fuzzing behavior
    size_t data_offset = 1;
    
    // 1. Test curl_multi_get_offt
    if (data_offset < size) {
        curl_off_t value;
        CURLMinfo_offt info_type;
        
        // Choose info type based on fuzzer data
        switch (data[data_offset] % 4) {
            case 0: info_type = CURLMINFO_XFERS_RUNNING; break;
            case 1: info_type = CURLMINFO_XFERS_PENDING; break;
            case 2: info_type = CURLMINFO_XFERS_DONE; break;
            default: info_type = CURLMINFO_XFERS_RUNNING; break;
        }
        data_offset = (data_offset + 1) % size;
        
        curl_multi_get_offt(multi_handle, info_type, &value);
    }

    // 2. Test curl_multi_timeout
    if (data_offset < size) {
        long milliseconds;
        curl_multi_timeout(multi_handle, &milliseconds);
        data_offset = (data_offset + 1) % size;
    }

    // 3. Test curl_multi_waitfds
    if (data_offset < size) {
        unsigned int fd_count;
        // First call to get count
        CURLMcode res = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
        
        if (res == CURLM_OK && fd_count > 0 && data_offset + 1 < size) {
            struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
            if (ufds) {
                // Initialize with fuzzer data
                for (unsigned int i = 0; i < fd_count && data_offset < size; i++) {
                    ufds[i].fd = (curl_socket_t)data[data_offset];
                    ufds[i].events = (short)(data[data_offset] & 0x03); // Use lower bits
                    ufds[i].revents = 0;
                    data_offset = (data_offset + 1) % size;
                }
                curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
                free(ufds);
            }
        }
    }

    // 4. Test curl_multi_wait
    if (data_offset < size) {
        int timeout_ms = (int)(data[data_offset] * 10); // 0-2550 ms
        data_offset = (data_offset + 1) % size;
        
        int ret;
        struct curl_waitfd extra_fds[2];
        unsigned int extra_nfds = 0;
        
        // Add some extra fds if we have data
        if (data_offset + 1 < size) {
            extra_fds[0].fd = (curl_socket_t)data[data_offset];
            extra_fds[0].events = (short)(data[data_offset] & 0x03);
            extra_fds[0].revents = 0;
            extra_nfds = 1;
            data_offset = (data_offset + 1) % size;
        }
        
        curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    }

    // 5. Test curl_multi_remove_handle
    if (data_offset < size && num_easy > 0) {
        int handle_idx = data[data_offset] % num_easy;
        data_offset = (data_offset + 1) % size;
        
        if (easy_handles[handle_idx]) {
            curl_multi_remove_handle(multi_handle, easy_handles[handle_idx]);
            // Re-add to keep state valid
            curl_multi_add_handle(multi_handle, easy_handles[handle_idx]);
        }
    }

    // 6. Test curl_multi_perform
    if (data_offset < size) {
        int running_handles;
        curl_multi_perform(multi_handle, &running_handles);
        data_offset = (data_offset + 1) % size;
        
        // Try multiple calls to explore different states
        for (int i = 0; i < 3 && data_offset < size; i++) {
            curl_multi_perform(multi_handle, &running_handles);
            data_offset = (data_offset + 1) % size;
        }
    }

    // Cleanup
    for (int i = 0; i < num_easy; i++) {
        if (easy_handles[i]) {
            curl_multi_remove_handle(multi_handle, easy_handles[i]);
            curl_easy_cleanup(easy_handles[i]);
        }
    }
    
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    dummy_file_cleanup();
    
    return 0;
}