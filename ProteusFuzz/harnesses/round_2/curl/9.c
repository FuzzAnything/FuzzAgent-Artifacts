// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_interleave_callback(curl_socket_t fd, int what, void *userp) {
    (void)fd;
    (void)what;
    (void)userp;
    return 0;
}

static int dummy_prereq_callback(void *userp) {
    (void)userp;
    return 0;
}

static int dummy_trailer_callback(char *key, size_t keylen, char *val, size_t vallen, void *userp) {
    (void)key;
    (void)keylen;
    (void)val;
    (void)vallen;
    (void)userp;
    return 0;
}

static uint8_t safe_get_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 9) {
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

    // Initialize a dummy file for file-based options if needed
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Fuzz curl_multi_strerror
    CURLMcode error_code = (CURLMcode)safe_get_byte(Data, Size, 0);
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        (void)strlen(error_str);
    }

    // Fuzz curl_multi_assign
    curl_socket_t sockfd = (curl_socket_t)safe_get_byte(Data, Size, 1);
    void *sockp = (void *)(uintptr_t)safe_get_byte(Data, Size, 2);
    CURLMcode assign_result = curl_multi_assign(multi_handle, sockfd, sockp);
    (void)assign_result;

    // Fuzz curl_multi_socket (deprecated, but still tested)
    int running_handles = 0;
    CURLMcode socket_result = curl_multi_socket(multi_handle, sockfd, &running_handles);
    (void)socket_result;

    // Fuzz curl_multi_perform
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    (void)perform_result;

    // Fuzz curl_multi_socket_action
    int ev_bitmask = (int)safe_get_byte(Data, Size, 3);
    CURLMcode socket_action_result = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
    (void)socket_action_result;

    // Set some options on the easy handle to explore more states
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
    
    // Use safe byte retrieval for data pointers
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)(uintptr_t)safe_get_byte(Data, Size, 4));
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)(uintptr_t)safe_get_byte(Data, Size, 5));
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEDATA, (void *)(uintptr_t)safe_get_byte(Data, Size, 6));
    curl_easy_setopt(easy_handle, CURLOPT_PREREQDATA, (void *)(uintptr_t)safe_get_byte(Data, Size, 7));
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERDATA, (void *)(uintptr_t)safe_get_byte(Data, Size, 8));

    // Add the easy handle to the multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Perform a few more operations to explore states
    curl_multi_perform(multi_handle, &running_handles);
    curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    if (dummy_file) {
        remove("./dummy_file");
    }

    return 0;
}