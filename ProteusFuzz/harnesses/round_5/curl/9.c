// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
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
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* Dummy callback implementations to satisfy type checks if needed */
static int dummy_opensocket_cb(void *userdata, curl_socket_t fd, struct curl_sockaddr *address) {
    (void)userdata;
    (void)fd;
    (void)address;
    return CURL_SOCKET_BAD;
}

static int dummy_closesocket_cb(void *userdata, curl_socket_t fd) {
    (void)userdata;
    (void)fd;
    return 0;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

static int dummy_debug_cb(CURL *curl, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)curl;
    (void)type;
    (void)data;
    (void)size;
    (void)userdata;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_xferinfo_cb(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_ssl_ctx_cb(void *clientp, void *ssl_ctx) {
    (void)clientp;
    (void)ssl_ctx;
    return 0;
}

static int dummy_conv_cb(void *clientp, char *in, size_t inlen, char *out, size_t *outlen) {
    (void)clientp;
    (void)in;
    (void)inlen;
    (void)out;
    (void)outlen;
    return 0;
}

static int dummy_seek_cb(void *clientp, curl_off_t offset, int origin) {
    (void)clientp;
    (void)offset;
    (void)origin;
    return 0;
}

static int dummy_chunk_bgn_cb(void *clientp, void *chunk) {
    (void)clientp;
    (void)chunk;
    return 0;
}

static int dummy_chunk_end_cb(void *clientp, void *chunk) {
    (void)clientp;
    (void)chunk;
    return 0;
}

static int dummy_fnmatch_cb(void *clientp, const char *pattern, const char *string) {
    (void)clientp;
    (void)pattern;
    (void)string;
    return 0;
}

static int dummy_hstsread_cb(void *clientp, const char *host, char *buffer, size_t buflen) {
    (void)clientp;
    (void)host;
    (void)buffer;
    (void)buflen;
    return 0;
}

static int dummy_hstswrite_cb(void *clientp, const char *host, const char *data, size_t datalen) {
    (void)clientp;
    (void)host;
    (void)data;
    (void)datalen;
    return 0;
}

static int dummy_ssh_key_cb(void *clientp, const char *host, int port, const char *fingerprint, int keytype, void *userdata) {
    (void)clientp;
    (void)host;
    (void)port;
    (void)fingerprint;
    (void)keytype;
    (void)userdata;
    return 0;
}

static int dummy_ssh_hostkey_cb(void *clientp, const char *host, int port, const char *key, size_t keylen, int keytype, void *userdata) {
    (void)clientp;
    (void)host;
    (void)port;
    (void)key;
    (void)keylen;
    (void)keytype;
    (void)userdata;
    return 0;
}

/* curl_interleave_callback is not a standard typedef in older curl versions or specific builds,
 * so we define it here if missing, or cast directly to the function pointer type expected.
 * The standard signature is: int (*)(void *, int, int, int)
 */
#ifndef CURL_INTERLEAVE_CALLBACK_DEFINED
typedef int (*curl_interleave_callback)(void *userdata, int read, int write, int error);
#define CURL_INTERLEAVE_CALLBACK_DEFINED
#endif

static int dummy_interleave_cb(void *clientp, int read, int write, int error) {
    (void)clientp;
    (void)read;
    (void)write;
    (void)error;
    return 0;
}

static int dummy_prereq_cb(void *clientp, CURL *curl, void *userdata) {
    (void)clientp;
    (void)curl;
    (void)userdata;
    return 0;
}

/* Use the standard typedef from curl.h, do not redefine it.
 * Signature: int (*)(struct curl_slist **, void *)
 */
static int dummy_trailer_cb(struct curl_slist **list, void *clientp) {
    (void)list;
    (void)clientp;
    return 0;
}

static int dummy_resolver_start_cb(void *clientp, const char *hostname, int flags, void *userdata) {
    (void)clientp;
    (void)hostname;
    (void)flags;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
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

    /* Fuzz curl_multi_assign */
    if (Size >= 4) {
        curl_socket_t sockfd = *(const int32_t *)(Data);
        void *sockp = (void *)(uintptr_t)Data[3];
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    /* Fuzz curl_multi_remove_handle */
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Fuzz curl_multi_strerror with various error codes */
    if (Size >= 1) {
        CURLMcode error_code = (CURLMcode)Data[0];
        const char *str = curl_multi_strerror(error_code);
        (void)str; /* Use the result to avoid unused warning */
    }

    /* Fuzz curl_easy_setopt with various callback types to trigger typecheck warnings */
    /* Note: These are just to exercise the typecheck macros, not to actually set valid options */
    curl_easy_setopt(easy_handle, CURLOPT_OPENSOCKETFUNCTION, (curl_opensocket_callback)dummy_opensocket_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, (curl_closesocket_callback)dummy_closesocket_cb);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, (curl_read_callback)dummy_read_cb);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, (curl_write_callback)dummy_write_cb);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, (curl_debug_callback)dummy_debug_cb);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, (curl_progress_callback)dummy_progress_cb);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, (curl_xferinfo_callback)dummy_xferinfo_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_FUNCTION, (curl_ssl_ctx_callback)dummy_ssl_ctx_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, (curl_conv_callback)dummy_conv_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, (curl_conv_callback)dummy_conv_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, (curl_conv_callback)dummy_conv_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, (curl_seek_callback)dummy_seek_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_BGN_FUNCTION, (curl_chunk_bgn_callback)dummy_chunk_bgn_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_END_FUNCTION, (curl_chunk_end_callback)dummy_chunk_end_cb);
    curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, (curl_fnmatch_callback)dummy_fnmatch_cb);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, (curl_hstsread_callback)dummy_hstsread_cb);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, (curl_hstswrite_callback)dummy_hstswrite_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, (curl_sshkeycallback)dummy_ssh_key_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, (curl_sshhostkeycallback)dummy_ssh_hostkey_cb);
    
    /* Use the typedef defined above or direct cast if typedef is missing in system headers */
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, (curl_interleave_callback)dummy_interleave_cb);
    
    curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, (curl_prereq_callback)dummy_prereq_cb);
    /* Use the standard curl_trailer_callback type */
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, (curl_trailer_callback)dummy_trailer_cb);
    curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_FUNCTION, (curl_resolver_start_callback)dummy_resolver_start_cb);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}