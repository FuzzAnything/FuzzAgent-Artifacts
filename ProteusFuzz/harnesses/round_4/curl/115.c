// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
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
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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
#include <errno.h>

/* 
 * The target functions (Wcurl_easy_setopt_err_ssh_key_cb, etc.) are compile-time
 * type-checking macros defined in typecheck-gcc.h. They do not have runtime logic
 * to fuzz directly. Instead, this driver fuzzes the underlying curl_easy_setopt
 * and curl_easy_getinfo functions with diverse inputs to exercise the code paths
 * that these macros are designed to protect.
 */

static size_t dummy_write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userdata;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    (void)clientp;
    (void)curlfd;
    (void)purpose;
    return 0;
}

static int dummy_opensocket_callback(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp;
    (void)purpose;
    (void)address;
    return CURL_SOCKET_BAD;
}

static int dummy_closesocket_callback(void *clientp, curl_socket_t curlfd) {
    (void)clientp;
    (void)curlfd;
    return 0;
}

static int dummy_ssh_key_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userdata) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)userdata;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userdata) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)userdata;
    return CURLKHSTAT_FINE;
}

static int dummy_interleave_cb(CURL *easy, int what, void *userdata) {
    (void)easy;
    (void)what;
    (void)userdata;
    return 0;
}

static int dummy_prereq_cb(CURL *easy, void *userdata) {
    (void)easy;
    (void)userdata;
    return 0;
}

static int dummy_trailer_cb(CURL *easy, char *data, size_t size, void *userdata) {
    (void)easy;
    (void)data;
    (void)size;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));

    /* Parse input to determine which operations to perform */
    int op_mode = Data[0] % 10;
    size_t offset = 1;
    size_t remaining = Size - offset;

    /* Fuzzing various setopt calls */
    if (op_mode < 3) {
        /* String options */
        if (remaining > 0) {
            char *url = (char *)malloc(remaining + 1);
            if (url) {
                memcpy(url, Data + offset, remaining);
                url[remaining] = '\0';
                curl_easy_setopt(curl, CURLOPT_URL, url);
                free(url);
            }
        }
    } else if (op_mode < 5) {
        /* Long options */
        long val = 0;
        if (remaining >= sizeof(long)) {
            memcpy(&val, Data + offset, sizeof(long));
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, val);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, val);
        }
    } else if (op_mode < 7) {
        /* Callback options */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
        curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
        curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
        
        /* SSH Key Callbacks (Targeting Wcurl_easy_setopt_err_ssh_key_cb logic) */
        curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_cb);
        curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, (void *)Data);
        
        /* SSH Hostkey Callbacks */
        curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);
        curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void *)Data);

        /* Interleave Callback */
        curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_cb);
        curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void *)Data);

        /* Prereq Callback */
        curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
        curl_easy_setopt(curl, CURLOPT_PREREQDATA, (void *)Data);

        /* Trailer Callback */
        curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
        curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void *)Data);
    } else if (op_mode < 8) {
        /* slist options (Targeting Wcurl_easy_setopt_err_curl_slist logic) */
        struct curl_slist *list = NULL;
        if (remaining > 0) {
            char *header = (char *)malloc(remaining + 1);
            if (header) {
                memcpy(header, Data + offset, remaining);
                header[remaining] = '\0';
                list = curl_slist_append(list, header);
                free(header);
            }
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_slist_free_all(list);
    } else if (op_mode < 9) {
        /* POST fields (Targeting Wcurl_easy_setopt_err_postfields logic) */
        if (remaining > 0) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)(Data + offset));
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, remaining);
        }
    } else {
        /* Error buffer and other options */
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }

    /* Fuzzing getinfo calls */
    if (Size > 2) {
        int info_mode = Data[1] % 5;
        
        if (info_mode == 0) {
            /* String info */
            char *str_ptr = NULL;
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_ptr);
        } else if (info_mode == 1) {
            /* Long info */
            long long_val = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
        } else if (info_mode == 2) {
            /* Double info */
            double double_val = 0.0;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_val);
        } else if (info_mode == 3) {
            /* Socket info (Targeting Wcurl_easy_getinfo_err_curl_socket logic) */
            curl_socket_t sock = CURL_SOCKET_BAD;
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock);
        } else if (info_mode == 4) {
            /* Certinfo (Targeting Wcurl_easy_getinfo_err_curl_certinfo logic) */
            struct curl_certinfo *certinfo = NULL;
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
            if (certinfo) {
                /* Cleanup if needed, though usually curl manages this */
            }
        }
    }

    /* Perform a dummy perform to trigger internal state changes */
    /* We don't check the return code strictly as we are fuzzing, but we avoid crashes */
    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    return 0;
}