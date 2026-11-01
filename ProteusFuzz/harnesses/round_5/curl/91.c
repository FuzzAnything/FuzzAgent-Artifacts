// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/multi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_resolver_start_callback(CURL *handle, void *clientp) {
    (void)handle;
    (void)clientp;
    return 0;
}

static int dummy_close_socket_callback(CURL *handle, curl_socket_t s, void *clientp) {
    (void)handle;
    (void)s;
    (void)clientp;
    return 0;
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userp) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userp;
    return 0;
}

static struct curl_slist *create_slist_from_data(const uint8_t *data, size_t size) {
    struct curl_slist *list = NULL;
    size_t offset = 0;
    
    while (offset < size) {
        size_t len = 0;
        // Find a null terminator or end of data
        while (offset + len < size && data[offset + len] != '\0') {
            len++;
        }
        
        if (len > 0) {
            char *str = malloc(len + 1);
            if (str) {
                memcpy(str, data + offset, len);
                str[len] = '\0';
                list = curl_slist_append(list, str);
                free(str);
            }
        }
        
        offset += len + 1; // Skip the null terminator if present
    }
    return list;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize CURLM handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Initialize CURL handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Extract options from fuzzing data
    int option_type = 0;
    if (Size > 0) option_type = Data[0] % 10; // Use first byte to determine option type

    // Prepare dummy socket
    curl_socket_t dummy_socket = -1;
    if (Size > 1) {
        dummy_socket = (curl_socket_t)(Data[1]);
    }

    int running_handles = 0;

    // Fuzz curl_multi_socket (Deprecated)
    if (option_type == 0) {
        curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    }

    // Fuzz curl_multi_socket_all (Deprecated)
    if (option_type == 1) {
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    // Fuzz CURLOPT_RESOLVER_START_FUNCTION (associated with Wcurl_easy_setopt_err_resolver_start_callback)
    if (option_type == 2) {
        curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_callback);
        curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_DATA, (void*)(Data + 2));
    }

    // Fuzz CURLOPT_CLOSESOCKETFUNCTION (associated with Wcurl_easy_setopt_err_close_socket_cb)
    if (option_type == 3) {
        curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_callback);
        curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETDATA, (void*)(Data + 2));
    }

    // Fuzz CURLOPT_READFUNCTION (associated with Wcurl_easy_setopt_err_read_cb)
    if (option_type == 4) {
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void*)(Data + 2));
    }

    // Fuzz CURLOPT_HTTPHEADER (associated with Wcurl_easy_setopt_err_curl_slist)
    if (option_type == 5) {
        struct curl_slist *headers = create_slist_from_data(Data + 2, Size > 2 ? Size - 2 : 0);
        if (headers) {
            curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
            curl_slist_free_all(headers);
        }
    }

    // Fuzz CURLOPT_POSTFIELDS (associated with Wcurl_easy_setopt_err_postfields)
    if (option_type == 6) {
        if (Size > 2) {
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (char*)(Data + 2));
        }
    }

    // Fuzz CURLOPT_URL with raw data (ensure null termination)
    if (option_type == 7) {
        char url[256];
        size_t url_len = Size > 255 ? 255 : Size;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    }

    // Perform a simple getinfo to exercise related paths
    long response_code = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}