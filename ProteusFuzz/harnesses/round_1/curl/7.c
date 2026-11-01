// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type checking */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *sslctx, void *userptr) {
    (void)curl; (void)sslctx; (void)userptr;
    return 0;
}

static int dummy_conv_callback(char *ptr, size_t inoutsize, void *userptr) {
    (void)ptr; (void)inoutsize; (void)userptr;
    return 0;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

static int dummy_chunk_bgn_callback(void *chunk, int num, void *ptr) {
    (void)chunk; (void)num; (void)ptr;
    return 0;
}

static int dummy_chunk_end_callback(void *chunk, void *ptr) {
    (void)chunk; (void)ptr;
    return 0;
}

static int dummy_close_socket_callback(curl_socket_t sockfd, void *blarg) {
    (void)sockfd; (void)blarg;
    return 0;
}

static int dummy_fnmatch_callback(void *userptr, const char *pattern, const char *string) {
    (void)userptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_hstsread_callback(void *userptr, char *line, size_t len) {
    (void)userptr; (void)line; (void)len;
    return 0;
}

static int dummy_hstswrite_callback(void *userptr, const char *line, size_t len) {
    (void)userptr; (void)line; (void)len;
    return 0;
}

static int dummy_ssh_key_callback(void *keyinfo, void *userdata) {
    (void)keyinfo; (void)userdata;
    return 0;
}

static int dummy_ssh_hostkey_callback(void *keyinfo, void *userdata) {
    (void)keyinfo; (void)userdata;
    return 0;
}

static int dummy_interleave_callback(curl_socket_t sockfd, int what, void *userp) {
    (void)sockfd; (void)what; (void)userp;
    return 0;
}

static int dummy_prereq_callback(void *req, void *userdata) {
    (void)req; (void)userdata;
    return 0;
}

static int dummy_trailer_callback(char *buf, size_t size, void *userdata) {
    (void)buf; (void)size; (void)userdata;
    return 0;
}

/* Helper to create a null-terminated string from fuzz data */
static char *create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    CURLSH *share = curl_share_init();
    if (!share) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Fuzz curl_share_setopt */
    /* Avoid passing invalid options that might cause immediate crashes or undefined behavior */
    if (Size > 0) {
        /* Use a safe subset of options to avoid invalid pointer dereferences */
        CURLSHoption option = (CURLSHoption)(Data[0] % 3); 
        /* Pass a valid pointer or NULL, avoid low integer values that look like pointers */
        void *dummy_val = (void*)(uintptr_t)(Data[0] | 0x1000); 
        curl_share_setopt(share, option, dummy_val);
    }

    /* Fuzz curl_easy_setopt with various option types */
    /* String options */
    if (Size > 1) {
        char *str_data = create_null_terminated_string(Data, Size);
        if (str_data) {
            curl_easy_setopt(curl, CURLOPT_URL, str_data);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_data);
            curl_easy_setopt(curl, CURLOPT_PROXY, str_data);
            curl_easy_setopt(curl, CURLOPT_SSLCERT, str_data);
            curl_easy_setopt(curl, CURLOPT_CAINFO, str_data);
            free(str_data);
        }
    }

    /* Long options */
    if (Size > 1) {
        long long_val = (long)(Data[0] << 8 | Data[1]);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, long_val);
    }

    /* Callback options */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_callback);
    curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);

    /* Data pointer options - Use a static buffer or allocated memory to avoid dangling pointers */
    static char static_buffer[256];
    memset(static_buffer, 0, sizeof(static_buffer));
    size_t copy_len = Size < 256 ? Size : 256;
    memcpy(static_buffer, Data, copy_len);
    curl_easy_setopt(curl, CURLOPT_READDATA, static_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_buffer);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, static_buffer);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, static_buffer);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, static_buffer);

    /* Error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, CURL_ERROR_SIZE);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* Slist options */
    struct curl_slist *slist = NULL;
    if (Size > 2) {
        char *str1 = create_null_terminated_string(Data, Size/2);
        char *str2 = create_null_terminated_string(Data + Size/2, Size - Size/2);
        if (str1 && str2) {
            slist = curl_slist_append(slist, str1);
            slist = curl_slist_append(slist, str2);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
            curl_easy_setopt(curl, CURLOPT_QUOTE, slist);
            curl_easy_setopt(curl, CURLOPT_POSTQUOTE, slist);
            curl_easy_setopt(curl, CURLOPT_PREQUOTE, slist);
            curl_easy_setopt(curl, CURLOPT_RESOLVE, slist);
            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, slist);
            curl_easy_setopt(curl, CURLOPT_PROXYHEADER, slist);
            curl_easy_setopt(curl, CURLOPT_TELNETOPTIONS, slist);
            curl_easy_setopt(curl, CURLOPT_CONNECT_TO, slist);
            curl_easy_setopt(curl, CURLOPT_HTTP200ALIASES, slist);
            free(str1);
            free(str2);
        }
    }

    /* Postfields - Must be null-terminated string for CURLOPT_COPYPOSTFIELDS */
    if (Size > 2) {
        char *post_data = create_null_terminated_string(Data, Size);
        if (post_data) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
            curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_data);
            free(post_data);
        }
    }

    /* Share option */
    curl_easy_setopt(curl, CURLOPT_SHARE, share);

    /* Cleanup */
    if (slist) curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
    curl_share_cleanup(share);

    return 0;
}