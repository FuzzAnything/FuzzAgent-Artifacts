// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Define CURL_DISABLE_TYPECHECK to avoid typecheck-gcc.h issues */
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
        
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    
    if (access("./dummy_file", F_OK) == 0) {
        dummy_socket = open("./dummy_file", O_RDONLY);
    }
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            if (dummy_socket >= 0) {
                curl_multi_socket_action(multi_handle, dummy_socket, 
                                         Data[1 % Size] & (CURL_CSELECT_IN | CURL_CSELECT_OUT | CURL_CSELECT_ERR),
                                         &running_handles);
            }
            break;
        case 1: {
            int dummy_data = 42;
            if (dummy_socket >= 0) {
                curl_multi_assign(multi_handle, dummy_socket, &dummy_data);
            }
            break;
        }
        case 2:
            if (Size > 1) {
                CURLMoption option = (CURLMoption)(Data[1 % Size] % 10);
                switch (option) {
                    case CURLMOPT_SOCKETFUNCTION:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                        break;
                    case CURLMOPT_SOCKETDATA:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, NULL);
                        break;
                    case CURLMOPT_TIMERFUNCTION:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                        break;
                    case CURLMOPT_TIMERDATA:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, NULL);
                        break;
                    case CURLMOPT_MAXCONNECTS:
                        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Data[2 % Size]));
                        break;
                    case CURLMOPT_MAX_TOTAL_CONNECTIONS:
                        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)(Data[2 % Size]));
                        break;
                    case CURLMOPT_PIPELINING:
                        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Data[2 % Size] & 1));
                        break;
                    case CURLMOPT_MAX_PIPELINE_LENGTH:
                        curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, (long)(Data[2 % Size]));
                        break;
                    case CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE:
                        curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, (curl_off_t)(Data[2 % Size]));
                        break;
                    case CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE:
                        curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, (curl_off_t)(Data[2 % Size]));
                        break;
                    default:
                        break;
                }
            }
            break;
        case 3:
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 4:
            if (dummy_socket >= 0) {
                curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            }
            break;
        case 5:
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
    }
    
    if (dummy_socket >= 0) {
        close(dummy_socket);
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    unlink("./dummy_file");
    
    return 0;
}