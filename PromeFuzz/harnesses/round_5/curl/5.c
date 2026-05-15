// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

/* Dummy callbacks for socket and timer functions */
static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what,
                                 void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

/* Write fuzzer data to a dummy file */
static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles;
    long timeout_val;
    curl_off_t off_val;
    
    /* Initialize multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    /* Initialize easy handle with dummy URL */
    easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Write fuzzer data to file and use as URL */
        write_dummy_file(data, size);
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        
        /* Add easy handle to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    /* Use fuzzer data to determine which functions to call */
    if (size > 0) {
        uint8_t selector = data[0];
        
        /* Call curl_multi_setopt with various options */
        if (selector & 0x01) {
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, NULL);
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, NULL);
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(size % 100));
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(size % 50));
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)(size % 200));
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)((selector >> 1) & 0x03));
            curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, (long)(size % 4096));
            curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, (long)(size % 8192));
        }
        
        /* Call curl_multi_perform */
        if (selector & 0x02) {
            curl_multi_perform(multi_handle, &running_handles);
        }
        
        /* Call curl_multi_socket_action with various events */
        if (selector & 0x04) {
            int ev_bitmask = 0;
            if (size > 1) ev_bitmask = data[1] & 0x07; /* CURL_CSELECT_IN, OUT, ERR */
            curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, ev_bitmask, &running_handles);
            
            /* Try with a dummy socket descriptor */
            curl_socket_t dummy_sock = (curl_socket_t)(size % 1024);
            curl_multi_socket_action(multi_handle, dummy_sock, ev_bitmask, &running_handles);
        }
        
        /* Call curl_multi_timeout */
        if (selector & 0x08) {
            curl_multi_timeout(multi_handle, &timeout_val);
        }
        
        /* Call curl_multi_get_offt with various info types */
        if (selector & 0x10) {
            CURLMinfo_offt info_type = (CURLMinfo_offt)((size > 2 ? data[2] : 0) % 4);
            switch (info_type) {
                case CURLMINFO_XFERS_RUNNING:
                case CURLMINFO_XFERS_PENDING:
                case CURLMINFO_XFERS_DONE:
                case CURLMINFO_XFERS_CURRENT:
                    curl_multi_get_offt(multi_handle, info_type, &off_val);
                    break;
                default:
                    break;
            }
        }
        
        /* Call curl_multi_remove_handle if easy handle exists */
        if ((selector & 0x20) && easy_handle) {
            curl_multi_remove_handle(multi_handle, easy_handle);
            
            /* Re-add to test multiple remove/add cycles */
            if (size > 3 && (data[3] & 0x01)) {
                curl_multi_add_handle(multi_handle, easy_handle);
            }
        }
        
        /* Perform again after potential changes */
        if (selector & 0x40) {
            curl_multi_perform(multi_handle, &running_handles);
        }
        
        /* Test socket action again with different parameters */
        if (selector & 0x80) {
            int ev_bitmask = (size > 4 ? data[4] : 0) & 0x07;
            curl_socket_t dummy_sock = (curl_socket_t)((size > 5 ? data[5] : 0) % 2048);
            curl_multi_socket_action(multi_handle, dummy_sock, ev_bitmask, &running_handles);
        }
    }
    
    /* Cleanup */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    
    /* Remove dummy file */
    unlink("./dummy_file");
    
    return 0;
}