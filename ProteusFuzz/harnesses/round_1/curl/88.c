// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Custom memory callbacks for curl_global_init_mem */
static void *fuzz_malloc(size_t size) {
    return malloc(size);
}

static void fuzz_free(void *ptr) {
    free(ptr);
}

static void *fuzz_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *fuzz_strdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *new_str = malloc(len);
    if (new_str) {
        memcpy(new_str, str, len);
    }
    return new_str;
}

static void *fuzz_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

/* Callback definitions must be outside LLVMFuzzerTestOneInput */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    return 0;
}

static int dummy_xferinfo_cb(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    return 0;
}

static int dummy_sockopt_cb(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    return 0;
}

static curl_socket_t dummy_opensocket_cb(void *clientp, struct curl_sockaddr *address) {
    return CURL_SOCKET_BAD;
}

static int dummy_closesocket_cb(void *clientp, curl_socket_t curlfd) {
    return 0;
}

static curlioerr dummy_ioctl_cb(CURL *handle, int cmd, void *clientp) {
    return CURLIOE_OK;
}

static int dummy_seek_cb(void *clientp, curl_off_t offset, int origin) {
    return 0;
}

static int dummy_conv_to_network_cb(void *clientp, char *data, size_t length) {
    return 0;
}

static int dummy_conv_from_network_cb(void *clientp, char *data, size_t length) {
    return 0;
}

static int dummy_conv_from_utf8_cb(void *clientp, char *data, size_t length) {
    return 0;
}

static int dummy_chunk_bgn_cb(void *clientp, int chunks_left, int chunk_start, void *first, void *outreq) {
    return 0;
}

static int dummy_chunk_end_cb(void *clientp, void *chunk) {
    return 0;
}

static int dummy_prereq_cb(void *clientp, void *req, void *outreq) {
    return 0;
}

static int dummy_interleave_cb(void *clientp, int read, int write, int error) {
    return 0;
}

static int dummy_trailer_cb(void *clientp, char *data, size_t size) {
    return 0;
}

static int dummy_hstsread_cb(void *clientp, char *buf, size_t size) {
    return 0;
}

static int dummy_hstswrite_cb(void *clientp, const char *line, size_t len) {
    return 0;
}

static int dummy_resolver_start_cb(void *clientp, curl_resolver_start_callback callback, void *resolver_start_data) {
    return 0;
}

/* Fixed: Use struct curl_khkey instead of curl_khkey */
static int dummy_ssh_key_cb(void *clientp, const char *host, int port, const char *fingerprint, struct curl_khkey fingerprint_type, const char *key, void *key_data) {
    return CURLKHSTAT_FINE;
}

/* Fixed: Use struct curl_khkey instead of curl_khkey */
static int dummy_ssh_hostkey_cb(void *clientp, const char *host, int port, const char *key, size_t keylen, struct curl_khkey keytype, const char *key_data, void *key_data_ptr) {
    return CURLKHSTAT_FINE;
}

static int dummy_ssl_ctx_cb(CURL *curl, void *ssl_ctx, void *userptr) {
    return 0;
}

static int dummy_fnmatch_cb(void *clientp, const char *pattern, const char *string) {
    return 0;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    long flags = 0;
    int init_type = 0;

    /* Ensure we have enough data to make decisions */
    if (Size < 1) {
        return 0;
    }

    /* Determine initialization strategy based on input */
    init_type = Data[0] % 3;

    /* Initialize libcurl globally */
    if (init_type == 0) {
        /* Use curl_global_init with flags derived from input */
        if (Size > 1) {
            flags = (long)(Data[1] & 0xFF);
        }
        res = curl_global_init(flags);
        if (res != CURLE_OK) {
            /* Initialization failed, but we continue to test error paths */
        }
    } else if (init_type == 1) {
        /* Use curl_global_init_mem with custom callbacks */
        if (Size > 1) {
            flags = (long)(Data[1] & 0xFF);
        }
        res = curl_global_init_mem(flags, fuzz_malloc, fuzz_free,
                                   fuzz_realloc, fuzz_strdup, fuzz_calloc);
        if (res != CURLE_OK) {
            /* Initialization failed */
        }
    } else {
        /* No global init, test error handling paths */
    }

    /* Create a CURL handle */
    curl = curl_easy_init();
    if (curl) {
        /* Set various options based on input data to explore states */
        if (Size > 2) {
            /* Set URL if possible */
            char url[256];
            size_t url_len = (Size > 256) ? 255 : Size - 2;
            memcpy(url, &Data[2], url_len);
            url[url_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
        }

        /* Set other options to trigger different code paths */
        if (Size > 3) {
            long timeout = (long)(Data[3] & 0xFF);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        }

        if (Size > 4) {
            long connect_timeout = (long)(Data[4] & 0xFF);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
        }

        /* Test CURLOPT_WRITEFUNCTION with a dummy callback */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);

        /* Test CURLOPT_READFUNCTION with a dummy callback */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);

        /* Test CURLOPT_HEADERFUNCTION */
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dummy_write_cb);

        /* Test CURLOPT_DEBUGFUNCTION */
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);

        /* Test CURLOPT_PROGRESSFUNCTION */
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);

        /* Test CURLOPT_XFERINFOFUNCTION */
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);

        /* Test CURLOPT_SOCKOPTFUNCTION */
        curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);

        /* Test CURLOPT_OPENSOCKETFUNCTION */
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_cb);

        /* Test CURLOPT_CLOSESOCKETFUNCTION */
        curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_cb);

        /* Test CURLOPT_IOCTLFUNCTION */
        curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, dummy_ioctl_cb);

        /* Test CURLOPT_SEEKFUNCTION */
        curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);

        /* Test CURLOPT_CONV_TO_NETWORK_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_to_network_cb);

        /* Test CURLOPT_CONV_FROM_NETWORK_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_from_network_cb);

        /* Test CURLOPT_CONV_FROM_UTF8_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_from_utf8_cb);

        /* Test CURLOPT_CHUNK_BGN_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_cb);

        /* Test CURLOPT_CHUNK_END_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);

        /* Fixed: CURLOPT_PREREQFUNCTION instead of CURLOPT_PREREQ_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);

        /* Test CURLOPT_INTERLEAVEFUNCTION */
        curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_cb);

        /* Test CURLOPT_TRAILERFUNCTION */
        curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);

        /* Test CURLOPT_HSTSREADFUNCTION */
        curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);

        /* Test CURLOPT_HSTSWRITEFUNCTION */
        curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);

        /* Test CURLOPT_RESOLVER_START_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_cb);

        /* Test CURLOPT_SSH_KEYFUNCTION */
        curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_cb);

        /* Test CURLOPT_SSH_HOSTKEYFUNCTION */
        curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);

        /* Test CURLOPT_SSL_CTX_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);

        /* Test CURLOPT_FNMATCH_FUNCTION */
        curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_cb);

        /* Perform upkeep on the handle */
        res = curl_easy_upkeep(curl);
        (void)res; /* Ignore return value for fuzzing */

        /* Cleanup the handle */
        curl_easy_cleanup(curl);
    }

    /* Cleanup global initialization */
    curl_global_cleanup();

    return 0;
}