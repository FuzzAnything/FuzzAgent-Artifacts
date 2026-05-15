// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(void) {}

static CURL *create_dummy_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_callback);
        curl_easy_setopt(easy, CURLOPT_HEADERFUNCTION, dummy_callback);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLM *multi = curl_multi_init();
    if (!multi) return 0;
    
    CURL *easy1 = create_dummy_easy_handle();
    CURL *easy2 = create_dummy_easy_handle();
    
    if (easy1) curl_multi_add_handle(multi, easy1);
    if (easy2) curl_multi_add_handle(multi, easy2);
    
    unsigned int seed = 0;
    if (Size >= sizeof(seed)) {
        memcpy(&seed, Data, sizeof(seed));
    } else {
        memcpy(&seed, Data, Size);
    }
    
    srand(seed);
    
    for (size_t i = 0; i < 5 && i < Size; i++) {
        uint8_t action = Data[i] % 6;
        
        switch (action) {
            case 0: {
                unsigned int fd_count = 0;
                curl_multi_waitfds(multi, NULL, 0, &fd_count);
                
                if (fd_count > 0 && fd_count < 100) {
                    struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                    if (ufds) {
                        curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
                        free(ufds);
                    }
                }
                break;
            }
            
            case 1: {
                CURLMinfo_offt info_types[] = {
                    CURLMINFO_XFERS_RUNNING,
                    CURLMINFO_XFERS_PENDING,
                    CURLMINFO_XFERS_DONE
                };
                CURLMinfo_offt info = info_types[Data[i] % 3];
                curl_off_t value = 0;
                curl_multi_get_offt(multi, info, &value);
                break;
            }
            
            case 2: {
                if (easy1) {
                    curl_multi_remove_handle(multi, easy1);
                    curl_multi_add_handle(multi, easy1);
                }
                if (easy2 && (Data[i] & 0x80)) {
                    curl_multi_remove_handle(multi, easy2);
                    curl_multi_add_handle(multi, easy2);
                }
                break;
            }
            
            case 3: {
                int extra_nfds = Data[i] % 4;
                struct curl_waitfd extra_fds[3];
                int ret = 0;
                
                for (int j = 0; j < extra_nfds; j++) {
                    extra_fds[j].fd = -1;
                    extra_fds[j].events = (Data[i] >> (j * 2)) & 0x03;
                    extra_fds[j].revents = 0;
                }
                
                int timeout_ms = (Data[i] % 100) * 10;
                curl_multi_wait(multi, extra_fds, extra_nfds, timeout_ms, &ret);
                break;
            }
            
            case 4: {
                long timeout = 0;
                curl_multi_timeout(multi, &timeout);
                break;
            }
            
            case 5: {
                int running = 0;
                curl_multi_perform(multi, &running);
                break;
            }
        }
    }
    
    if (easy1) {
        curl_multi_remove_handle(multi, easy1);
        curl_easy_cleanup(easy1);
    }
    if (easy2) {
        curl_multi_remove_handle(multi, easy2);
        curl_easy_cleanup(easy2);
    }
    
    curl_multi_cleanup(multi);
    return 0;
}