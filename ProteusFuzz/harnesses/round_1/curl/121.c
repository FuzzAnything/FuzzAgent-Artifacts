// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

/* 
 * The target functions listed (e.g., Wcurl_easy_getinfo_err_curl_socket) 
 * are compile-time warning macros defined in typecheck-gcc.h. 
 * They do not exist as runtime symbols. 
 * To fuzz the logic they guard, we must fuzz the actual runtime APIs:
 * curl_easy_setopt and curl_easy_getinfo, using the types and options 
 * that these macros are designed to validate.
 */

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

static int dummy_interleave_callback(CURL *easy, curl_socket_t fd, int what, void *clientp, void *socketp) {
    (void)easy;
    (void)fd;
    (void)what;
    (void)clientp;
    (void)socketp;
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

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize error buffer for CURLOPT_ERRORBUFFER */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));

    /* Parse input to determine which API to exercise */
    uint8_t mode = Data[0] % 10;

    /* Prepare variables for getinfo */
    long long_val = 0;
    double double_val = 0.0;
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    curl_off_t off_val = 0;
    char *string_val = NULL;
    struct curl_slist *slist_val = NULL;
    struct curl_certinfo *certinfo_val = NULL;
    struct curl_tlssessioninfo *tlssession_val = NULL;

    switch (mode) {
        case 0:
            /* Test CURLOPT_URL (string) */
            if (Size > 1) {
                char url[256];
                size_t len = (Size > 255) ? 255 : Size - 1;
                memcpy(url, Data + 1, len);
                url[len] = '\0';
                curl_easy_setopt(curl, CURLOPT_URL, url);
            }
            break;

        case 1:
            /* Test CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
            break;

        case 2:
            /* Test CURLOPT_READFUNCTION and CURLOPT_READDATA */
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
            break;

        case 3:
            /* Test CURLOPT_INTERLEAVEFUNCTION (guarded by Wcurl_easy_setopt_err_interleave_cb) */
            curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
            curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void *)Data);
            break;

        case 4:
            /* Test CURLOPT_ERRORBUFFER (guarded by Wcurl_easy_setopt_err_error_buffer) */
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
            break;

        case 5:
            /* Test CURLOPT_STREAM_DEPENDS (guarded by Wcurl_easy_setopt_err_curl) */
            /* Passing a CURL* as argument */
            curl_easy_setopt(curl, CURLOPT_STREAM_DEPENDS, curl);
            break;

        case 6:
            /* Test curl_easy_getinfo with various types */
            /* CURLINFO_RESPONSE_CODE (long) */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
            /* CURLINFO_TOTAL_TIME (double) */
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_val);
            /* CURLINFO_ACTIVESOCKET (curl_socket_t) - guarded by Wcurl_easy_getinfo_err_curl_socket */
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_val);
            /* CURLINFO_SIZE_DOWNLOAD_T (curl_off_t) - guarded by Wcurl_easy_getinfo_err_curl_off_t */
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
            /* CURLINFO_EFFECTIVE_URL (char *) */
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_val);
            /* CURLINFO_COOKIELIST (struct curl_slist *) */
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_val);
            /* CURLINFO_CERTINFO (struct curl_certinfo *) - guarded by Wcurl_easy_getinfo_err_curl_certinfo */
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo_val);
            /* CURLINFO_TLS_SESSION (struct curl_tlssessioninfo *) - guarded by Wcurl_easy_getinfo_err_curl_tlssessioninfo */
            curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssession_val);
            
            /* Cleanup slist */
            if (slist_val) {
                curl_slist_free_all(slist_val);
                slist_val = NULL;
            }
            /* Cleanup certinfo */
            if (certinfo_val) {
                /* curl_certinfo is opaque, but we don't own the memory, just free the list if needed? 
                   Actually curl_easy_getinfo returns a pointer to internal data or allocated list.
                   For CERTINFO, it's allocated. */
                /* Note: In real usage, one must free the certinfo structure if returned. 
                   However, for fuzzing stability, we just let it be freed by curl_easy_cleanup if possible, 
                   or ignore if it's internal. The API docs say curl_easy_cleanup frees it. */
            }
            break;

        case 7:
            /* Test CURLOPT_HTTPHEADER (struct curl_slist *) */
            if (Size > 1) {
                struct curl_slist *list = NULL;
                char header[64];
                size_t len = (Size > 63) ? 63 : Size - 1;
                memcpy(header, Data + 1, len);
                header[len] = '\0';
                list = curl_slist_append(list, header);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                curl_slist_free_all(list);
            }
            break;

        case 8:
            /* Test CURLOPT_POSTFIELDS */
            if (Size > 1) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)(Data + 1));
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size - 1);
            }
            break;

        case 9:
            /* Test CURLOPT_DEBUGFUNCTION */
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
            curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)Data);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            break;
    }

    /* Perform a dummy perform to trigger internal state changes if possible */
    /* We use a dummy URL to avoid network calls if possible, but curl might still try to connect.
       To prevent hanging, we set a very short timeout. */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    /* If no URL was set, set a dummy one to avoid "no URL" error in perform */
    if (mode != 0) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1");
    }

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    return 0;
}