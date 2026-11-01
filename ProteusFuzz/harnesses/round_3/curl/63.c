// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include curl headers */
#include <curl/curl.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_fnmatch_cb(void *ptr, const char *pattern, const char *string) {
    (void)ptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_seek_cb(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_OK;
}

static curl_socket_t dummy_sockopt_cb(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    (void)clientp; (void)curlfd; (void)purpose;
    return CURL_SOCKET_BAD;
}

static curl_socket_t dummy_opensocket_cb(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp; (void)purpose; (void)address;
    return CURL_SOCKET_BAD;
}

static int dummy_closesocket_cb(void *clientp, curl_socket_t curlfd) {
    (void)clientp; (void)curlfd;
    return 0;
}

static int dummy_ssh_key_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_interleave_cb(CURL *easy, int readwrite, void *clientp) {
    (void)easy; (void)readwrite; (void)clientp;
    return 0;
}

static int dummy_prereq_cb(CURL *easy, void *clientp) {
    (void)easy; (void)clientp;
    return 0;
}

static int dummy_trailer_cb(CURL *easy, char *data, size_t size, void *userp) {
    (void)easy; (void)data; (void)size; (void)userp;
    return 0;
}

static int dummy_ssl_ctx_cb(CURL *easy, void *ssl_ctx, void *userptr) {
    (void)easy; (void)ssl_ctx; (void)userptr;
    return 0;
}

static char *dummy_conv_cb(void *clientp, char *in, size_t inlen, size_t *outlen) {
    (void)clientp; (void)in; (void)inlen; (void)outlen;
    return NULL;
}

static int dummy_chunk_bgn_cb(CURL *easy, void *chunk, void *userdata) {
    (void)easy; (void)chunk; (void)userdata;
    return 0;
}

static int dummy_chunk_end_cb(CURL *easy, void *chunk, void *userdata) {
    (void)easy; (void)chunk; (void)userdata;
    return 0;
}

static int dummy_hstsread_cb(void *clientp, const char *domain, char *buffer, size_t buflen) {
    (void)clientp; (void)domain; (void)buffer; (void)buflen;
    return 0;
}

static int dummy_hstswrite_cb(void *clientp, const char *domain, const char *buffer, size_t buflen) {
    (void)clientp; (void)domain; (void)buffer; (void)buflen;
    return 0;
}

static int dummy_resolver_start_cb(CURL *easy, void *userdata) {
    (void)easy; (void)userdata;
    return 0;
}

static int dummy_xferinfo_cb(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Allocate error buffer dynamically to avoid stack issues */
    char *error_buffer = malloc(CURL_ERROR_SIZE);
    if (!error_buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memset(error_buffer, 0, CURL_ERROR_SIZE);

    /* Use fuzzer data to drive option selection and values */
    uint32_t option_idx = 0;
    if (Size >= 4) {
        option_idx = *(const uint32_t *)Data;
    }
    
    /* Limit option range to known valid options to avoid undefined behavior */
    CURLoption option = (CURLoption)(option_idx % 500); 

    /* Fuzz various option types */
    switch (option % 10) {
        case 0: /* Long options */
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(Size % 1000));
            break;
        case 1: /* String options - Ensure null termination */
            if (Size > 0) {
                /* Create a safe, null-terminated string from fuzzer input */
                /* We limit the length to avoid huge allocations or buffer issues */
                size_t safe_len = (Size > 1024) ? 1024 : Size;
                char *safe_str = malloc(safe_len + 1);
                if (safe_str) {
                    memcpy(safe_str, Data, safe_len);
                    safe_str[safe_len] = '\0';
                    curl_easy_setopt(curl, CURLOPT_URL, safe_str);
                    free(safe_str);
                }
            }
            break;
        case 2: /* Callback options */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
            break;
        case 3: /* Progress callback */
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
            break;
        case 4: /* Debug callback */
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
            break;
        case 5: /* Fnmatch callback */
            curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_cb);
            break;
        case 6: /* Seek callback */
            curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
            break;
        case 7: /* Sockopt callback */
            curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);
            break;
        case 8: /* Opensocket callback */
            curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_cb);
            break;
        case 9: /* Closesocket callback */
            curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_cb);
            break;
    }

    /* Additional diverse options */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_cb);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_cb);
    curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_cb);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_cb);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);

    /* Reset to explore different states */
    curl_easy_reset(curl);

    /* Cleanup */
    free(error_buffer);
    curl_easy_cleanup(curl);

    return 0;
}