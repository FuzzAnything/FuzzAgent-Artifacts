// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <unistd.h>
#include <fcntl.h>

static void cleanup_handles(CURLM *multi, CURL *easy) {
    if (multi && easy) {
        (void)curl_multi_remove_handle(multi, easy);
    }
    if (multi) {
        curl_multi_cleanup(multi);
    }
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
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

    // Initialize dummy file for potential file-based operations if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Fuzz curl_multi_setopt with various options and values derived from input
    CURLMcode res;
    int option_val = Data[0];
    int long_val = Data[1];
    void *ptr_val = (void *)(uintptr_t)(Data[0] + Data[1]);

    // Set some options to trigger different code paths
    // Note: curl_multi_setopt returns void, so we cannot assign to res directly
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, option_val);
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, long_val);

    // Fuzz curl_multi_assign
    curl_socket_t sockfd = (curl_socket_t)(Data[0] + Data[1]);
    res = curl_multi_assign(multi, sockfd, ptr_val);
    (void)res;

    // Fuzz curl_multi_remove_handle
    res = curl_multi_remove_handle(multi, easy);
    (void)res;

    // Fuzz curl_multi_wakeup
    res = curl_multi_wakeup(multi);
    (void)res;

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_code = (CURLMcode)(Data[0]);
    const char *err_str = curl_multi_strerror(error_code);
    if (err_str) {
        // Use the string to prevent optimization
        volatile const char *v = err_str;
        (void)v;
    }

    // Try adding the handle back to explore more states
    res = curl_multi_add_handle(multi, easy);
    if (res == CURLM_OK) {
        res = curl_multi_remove_handle(multi, easy);
        (void)res;
    }

    // Cleanup
    cleanup_handles(multi, easy);
    
    // Clean up dummy file
    unlink(dummy_file);

    return 0;
}