// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy data for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    create_dummy_file();

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Fuzz curl_multi_wakeup
    // Call with valid handle
    curl_multi_wakeup(multi);
    // Call with NULL to test error handling
    curl_multi_wakeup(NULL);

    // Fuzz curl_multi_assign
    // Use a dummy socket and pointer derived from input
    curl_socket_t sockfd = (Size > 0) ? (curl_socket_t)(Data[0] & 0xFF) : 0;
    void *sockp = (void *)(Size > 1 ? (uintptr_t)(Data[1] & 0xFF) : 0);
    curl_multi_assign(multi, sockfd, sockp);
    curl_multi_assign(NULL, sockfd, sockp);

    // Fuzz curl_multi_remove_handle
    // Add easy handle first to make removal meaningful
    curl_multi_add_handle(multi, easy);
    curl_multi_remove_handle(multi, easy);
    curl_multi_remove_handle(multi, NULL);
    curl_multi_remove_handle(NULL, easy);

    // Fuzz curl_multi_setopt
    // Try setting various options if available, using data from input
    if (Size > 2) {
        CURLMoption option = (CURLMoption)(Data[2] & 0xFF);
        // We pass a dummy value; the fuzzer explores different option codes
        curl_multi_setopt(multi, option, (void *)0x12345678);
        curl_multi_setopt(NULL, option, (void *)0x12345678);
    }

    // Fuzz curl_multi_strerror
    // Try various error codes derived from input
    if (Size > 3) {
        CURLMcode code = (CURLMcode)(Data[3] & 0xFF);
        const char *str = curl_multi_strerror(code);
        (void)str; // Use the result to avoid unused variable warning
    }

    // Cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}