// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

static void cleanup_handles(CURLM *multi, CURL *easy) {
    if (multi) {
        if (easy) {
            curl_multi_remove_handle(multi, easy);
        }
        curl_multi_cleanup(multi);
    }
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
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

    // Basic setup for easy handle to ensure validity
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi, easy) != CURLM_OK) {
        cleanup_handles(multi, easy);
        return 0;
    }

    // Fuzzing logic based on input data
    unsigned int op = Data[0] % 6;
    unsigned int opt_val = (Size > 1) ? Data[1] : 0;
    
    // Safe extraction of long value, ensuring bounds check
    long long_val = 0;
    if (Size > 3) {
        long_val = (long)(Data[2] | ((long)Data[3] << 8));
    } else if (Size > 1) {
        long_val = (long)Data[1];
    }
    
    void *ptr_val = NULL;
    if (Size > 4) {
        ptr_val = (void *)&Data[4];
    }

    switch (op) {
        case 0:
            // Test curl_multi_wakeup
            curl_multi_wakeup(multi);
            break;

        case 1:
            // Test curl_multi_setopt with various options
            if (opt_val == 0) {
                // CURLMOPT_PIPELINING (long)
                curl_multi_setopt(multi, CURLMOPT_PIPELINING, long_val);
            } else if (opt_val == 1) {
                // CURLMOPT_MAXCONNECTS (long)
                curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, long_val);
            } else {
                // CURLMOPT_MAX_HOST_CONNECTIONS (long)
                curl_multi_setopt(multi, CURLMOPT_MAX_HOST_CONNECTIONS, long_val);
            }
            break;

        case 2:
            // Test curl_multi_remove_handle
            curl_multi_remove_handle(multi, easy);
            break;

        case 3:
            // Test curl_multi_assign
            curl_socket_t sock = (curl_socket_t)(long_val & 0xFFFF);
            curl_multi_assign(multi, sock, ptr_val);
            break;

        case 4:
            // Test curl_multi_strerror
            CURLMcode err_code = (CURLMcode)(long_val & 0xFF);
            const char *err_str = curl_multi_strerror(err_code);
            if (err_str) {
                // Use the string to prevent unused variable warnings
                (void)strlen(err_str);
            }
            break;

        case 5:
            // Test curl_multi_setopt with another valid long option
            curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, long_val);
            break;
    }

    // Cleanup
    cleanup_handles(multi, easy);

    return 0;
}