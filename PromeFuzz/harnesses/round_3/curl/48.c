// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
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
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

static CURL *create_dummy_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/");
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(easy, CURLOPT_UPLOAD, 0L);
        curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, 1L);
        curl_easy_setopt(easy, CURLOPT_TIMEOUT, 1L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_TIME, 1L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    write_dummy_file(data, size);
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        cleanup_dummy_file();
        return 0;
    }
    
    CURL *easy_handle = create_dummy_easy_handle();
    if (easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    int selector = data[0] % 6;
    
    switch (selector) {
        case 0: {
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
        case 1: {
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        }
        case 2: {
            curl_socket_t sock = (curl_socket_t)(size > 1 ? data[1] : 0);
            curl_multi_socket(multi_handle, sock, &running_handles);
            break;
        }
        case 3: {
            curl_socket_t sock = (curl_socket_t)(size > 1 ? data[1] : 0);
            int ev_bitmask = size > 2 ? data[2] : 0;
            curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
            break;
        }
        case 4: {
            int opt_selector = size > 1 ? data[1] % 10 : 0;
            switch (opt_selector) {
                case 0: curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(size > 2 ? data[2] : 0)); break;
                case 1: curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(size > 2 ? data[2] : 0)); break;
                case 2: curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(size > 2 ? data[2] : 0)); break;
                case 3: curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, (long)(size > 2 ? data[2] : 0)); break;
                case 4: curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, (long)(size > 2 ? data[2] : 0)); break;
                case 5: curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, (long)(size > 2 ? data[2] : 0)); break;
                case 6: curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)(size > 2 ? data[2] : 0)); break;
                case 7: curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL); break;
                case 8: curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, NULL); break;
                case 9: curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL); break;
            }
            break;
        }
        case 5: {
            curl_socket_t sock = (curl_socket_t)(size > 1 ? data[1] : 0);
            void *sockp = size > 2 ? (void *)(intptr_t)data[2] : NULL;
            curl_multi_assign(multi_handle, sock, sockp);
            break;
        }
    }
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    cleanup_dummy_file();
    
    return 0;
}