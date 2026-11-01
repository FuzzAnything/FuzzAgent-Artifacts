// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static helper to create a simple slist from fuzz data */
static struct curl_slist *create_slist_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    /* Create a buffer with a null terminator */
    char *buf = malloc(size + 1);
    if (!buf) return NULL;
    
    memcpy(buf, data, size);
    buf[size] = '\0';
    
    /* Remove newlines to make it a single header line for simplicity */
    for (size_t i = 0; i < size; i++) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            buf[i] = ' ';
        }
    }
    
    return curl_slist_append(NULL, buf);
}

/* Static helper to cleanup slist */
static void cleanup_slist(struct curl_slist *list) {
    if (list) {
        curl_slist_free_all(list);
    }
}

/* Static helper to cleanup curl handle */
static void cleanup_handle(CURL *handle) {
    if (handle) {
        curl_easy_cleanup(handle);
    }
}

/* Dummy callback functions to satisfy type requirements */
static size_t dummy_write_cb(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_hstswrite_cb(void *userdata, const char *host, const char *path, const char *value) {
    (void)userdata; (void)host; (void)path; (void)value;
    return 0;
}

static int dummy_prereq_cb(void *userdata, int flags) {
    (void)userdata; (void)flags;
    return 0;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *handle = curl_easy_init();
    if (!handle) return 0;

    /* Initialize with a dummy URL to ensure basic setup */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, dummy_read_cb);

    /* Use fuzz data to drive various setopt/getinfo calls */
    
    /* 1. Test CURLOPT_HTTPHEADER (curl_slist type) */
    if (Size > 0) {
        struct curl_slist *headers = create_slist_from_data(Data, Size);
        if (headers) {
            curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
            /* Test getinfo for slist (CURLINFO_COOKIELIST) */
            struct curl_slist *cookie_list = NULL;
            curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &cookie_list);
            if (cookie_list) {
                curl_slist_free_all(cookie_list);
            }
            cleanup_slist(headers);
        }
    }

    /* 2. Test CURLOPT_HSTSWRITEFUNCTION (callback type) */
    curl_easy_setopt(handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
    curl_easy_setopt(handle, CURLOPT_HSTSWRITEDATA, (void*)Data);

    /* 3. Test CURLOPT_PREREQFUNCTION (callback type) */
    curl_easy_setopt(handle, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
    curl_easy_setopt(handle, CURLOPT_PREREQDATA, (void*)Data);

    /* 4. Test getinfo with double type (CURLINFO_SPEED_DOWNLOAD) */
    double download_speed;
    curl_easy_getinfo(handle, CURLINFO_SPEED_DOWNLOAD, &download_speed);

    /* 5. Test getinfo with curl_socket_t type (CURLINFO_ACTIVESOCKET) */
    curl_socket_t socket_fd;
    curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &socket_fd);

    /* 6. Test getinfo with curl_certinfo type (CURLINFO_CERTINFO) */
    struct curl_certinfo *certinfo;
    curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo);
    if (certinfo) {
        /* Note: In real code, we would free certinfo->certinfo[i], 
           but for fuzzing we just let curl handle it on cleanup */
    }

    /* 7. Test various string options using fuzz data */
    if (Size > 10) {
        char *str_buf = malloc(Size + 1);
        if (str_buf) {
            memcpy(str_buf, Data, Size);
            str_buf[Size] = '\0';
            
            /* Ensure it's a valid string for URL if we reuse it */
            if (str_buf[0] != 'h' && str_buf[0] != 'f' && str_buf[0] != 'm') {
                str_buf[0] = 'h'; str_buf[1] = 't'; str_buf[2] = 't'; str_buf[3] = 'p'; str_buf[4] = ':';
            }
            
            curl_easy_setopt(handle, CURLOPT_URL, str_buf);
            curl_easy_setopt(handle, CURLOPT_USERAGENT, str_buf);
            curl_easy_setopt(handle, CURLOPT_REFERER, str_buf);
            
            free(str_buf);
        }
    }

    /* 8. Test long options */
    long long_val = (Size > sizeof(long)) ? *(long*)Data : (long)Size;
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, long_val);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, long_val);

    /* 9. Test off_t options */
    curl_off_t off_val = (Size > sizeof(curl_off_t)) ? *(curl_off_t*)Data : (curl_off_t)Size;
    curl_easy_setopt(handle, CURLOPT_MAXFILESIZE, off_val);

    /* 10. Test getinfo with long type */
    long response_code;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response_code);

    /* Cleanup */
    cleanup_handle(handle);

    return 0;
}