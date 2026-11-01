// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static int push_callback(CURL *parent, CURL *easy, size_t num_headers,
                         struct curl_header *headers, void *userp) {
    (void)parent;
    (void)easy;
    (void)num_headers;
    (void)headers;
    (void)userp;
    return 1;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Prepare dummy file if needed
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Fuzz curl_multi_setopt with various options
    // Use data from input to select options and values
    int option_idx = Data[0] % 8;
    CURLMoption option;
    void *opt_value = NULL;
    long long_val = 0;

    // Ensure we have enough data before accessing Data[1]
    uint8_t byte_val = (Size > 1) ? Data[1] : 0;

    switch (option_idx) {
        case 0:
            option = CURLMOPT_PUSHFUNCTION;
            opt_value = (void *)push_callback;
            break;
        case 1:
            option = CURLMOPT_SOCKETFUNCTION;
            opt_value = NULL;
            break;
        case 2:
            option = CURLMOPT_TIMERFUNCTION;
            opt_value = NULL;
            break;
        case 3:
            option = CURLMOPT_PIPELINING;
            long_val = (long)(byte_val & 0xFF);
            opt_value = (void *)&long_val;
            break;
        case 4:
            option = CURLMOPT_MAXCONNECTS;
            long_val = (long)(byte_val & 0xFF);
            opt_value = (void *)&long_val;
            break;
        case 5:
            option = CURLMOPT_MAX_HOST_CONNECTIONS;
            long_val = (long)(byte_val & 0xFF);
            opt_value = (void *)&long_val;
            break;
        case 6:
            option = CURLMOPT_MAX_PIPELINE_LENGTH;
            long_val = (long)(byte_val & 0xFF);
            opt_value = (void *)&long_val;
            break;
        case 7:
            option = CURLMOPT_MAX_CONCURRENT_STREAMS;
            long_val = (long)(byte_val & 0xFF);
            opt_value = (void *)&long_val;
            break;
        default:
            option = CURLMOPT_PUSHFUNCTION;
            opt_value = (void *)push_callback;
            break;
    }

    // Invoke curl_multi_setopt
    if (option == CURLMOPT_PUSHFUNCTION) {
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, push_callback);
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    } else if (opt_value) {
        curl_multi_setopt(multi_handle, option, opt_value);
    } else {
        curl_multi_setopt(multi_handle, option, NULL);
    }

    // Fuzz curl_multi_strerror with various error codes
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLMcode error_code = (CURLMcode)(Data[i] & 0xFF);
        const char *error_str = curl_multi_strerror(error_code);
        if (error_str) {
            (void)strlen(error_str);
        }
    }

    // Fuzz curl_multi_assign
    curl_socket_t sockfd = (curl_socket_t)(Data[0] & 0xFF);
    void *sockp = (void *)&Data[0];
    curl_multi_assign(multi_handle, sockfd, sockp);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}