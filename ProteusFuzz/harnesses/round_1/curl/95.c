// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <curl/websockets.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;
    char error_buffer[CURL_ERROR_SIZE];
    long long_option;
    curl_off_t off_t_option;
    void *post_data = NULL;
    struct curl_slist *slist = NULL;

    // Initialize libcurl globally
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Basic setup
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    res = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    // Fuzzing logic with bounds checking
    if (Size >= 3) {
        switch (Data[0] % 10) {
            case 0: // Long option
                long_option = (long)(Data[0] ^ Data[1] ^ Data[2]);
                if (long_option > 0 && long_option < CURLOPTTYPE_OBJECTPOINT) {
                    curl_easy_setopt(curl, (CURLoption)long_option, long_option);
                }
                break;
            case 1: // String option
                if (Size > 2) {
                    size_t len = Size - 2;
                    if (len > 1024) len = 1024;
                    char *tmp_str = malloc(len + 1);
                    if (tmp_str) {
                        memcpy(tmp_str, &Data[2], len);
                        tmp_str[len] = '\0';
                        curl_easy_setopt(curl, CURLOPT_URL, tmp_str);
                        curl_easy_setopt(curl, CURLOPT_USERAGENT, tmp_str);
                        curl_easy_setopt(curl, CURLOPT_REFERER, tmp_str);
                        free(tmp_str);
                    }
                }
                break;
            case 2: // POST fields
                if (Size > 2) {
                    size_t len = Size - 2;
                    if (len > 4096) len = 4096;
                    post_data = malloc(len + 1);
                    if (post_data) {
                        memcpy(post_data, &Data[2], len);
                        ((char*)post_data)[len] = '\0';
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);
                    }
                }
                break;
            case 3: // SList option
                if (Size > 2) {
                    slist = curl_slist_append(slist, "Header1: Value1");
                    slist = curl_slist_append(slist, "Header2: Value2");
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
                }
                break;
            case 4: // Off_t option
                off_t_option = (curl_off_t)(Data[0] | (Data[1] << 8));
                if (off_t_option > CURLOPTTYPE_OFF_T && off_t_option < CURLOPTTYPE_BLOB) {
                    curl_easy_setopt(curl, (CURLoption)off_t_option, off_t_option);
                }
                break;
            case 5: // Callback data
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)Data);
                curl_easy_setopt(curl, CURLOPT_READDATA, (void*)Data);
                break;
            case 6: // Custom request
                if (Size > 4) {
                    char req[64];
                    size_t req_len = Size - 4;
                    if (req_len > 63) req_len = 63;
                    memcpy(req, &Data[4], req_len);
                    req[req_len] = '\0';
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req);
                }
                break;
            case 7: // Proxy
                if (Size > 4) {
                    char proxy[128];
                    size_t p_len = Size - 4;
                    if (p_len > 127) p_len = 127;
                    memcpy(proxy, &Data[4], p_len);
                    proxy[p_len] = '\0';
                    curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
                }
                break;
            case 8: // Timeout
                long timeout = (long)(Data[0] + Data[1]);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
                break;
            case 9: // Max connects
                long max_conn = (long)(Data[0] + Data[1]);
                curl_easy_setopt(curl, CURLOPT_MAXCONNECTS, max_conn);
                break;
        }
    }

    // Ensure URL is set for perform
    if (Size > 4) {
        char url[256];
        size_t u_len = Size - 4;
        if (u_len > 255) u_len = 255;
        memcpy(url, &Data[4], u_len);
        url[u_len] = '\0';
        static char static_url_buf[256];
        memcpy(static_url_buf, url, sizeof(static_url_buf));
        curl_easy_setopt(curl, CURLOPT_URL, static_url_buf);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1");
    }

    // Execute the transfer
    res = curl_easy_perform(curl);

    // Test curl_easy_upkeep
    res = curl_easy_upkeep(curl);

    // Test WebSocket functions
    unsigned int ws_flags = (Size > 0) ? Data[0] : 0;
    // Fixed: Check Size > 2 before accessing Data[2]
    curl_off_t ws_len = (Size > 2) ? (curl_off_t)(Data[1] | (Data[2] << 8)) : 0;
    res = curl_ws_start_frame(curl, ws_flags, ws_len);

    // Cleanup
    if (post_data) {
        free(post_data);
    }
    if (slist) {
        curl_slist_free_all(slist);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}