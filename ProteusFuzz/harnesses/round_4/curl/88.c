// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

static int dummy_socket = -1;

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

static int dummy_ioctl_callback(CURL *handle, int cmd, curl_off_t *clientp) {
    (void)handle;
    (void)cmd;
    (void)clientp;
    return CURLIOE_OK;
}

static int dummy_sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    (void)clientp;
    (void)curlfd;
    (void)purpose;
    return CURL_SOCKOPT_ALREADY_CONNECTED;
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

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
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

static int dummy_xferinfo_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_seek_callback(void *inclientp, curl_off_t offset, int origin) {
    (void)inclientp;
    (void)offset;
    (void)origin;
    return 0;
}

static int dummy_conv_callback(void *inclientp, char *in, size_t inlen, char *out, size_t outlen, size_t *outwritten) {
    (void)inclientp;
    (void)in;
    (void)inlen;
    (void)out;
    (void)outlen;
    (void)outwritten;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl;
    (void)ssl_ctx;
    (void)userptr;
    return 0;
}

static int dummy_chunk_bgn_callback(CURL *easy, void *chunk, void *userdata) {
    (void)easy;
    (void)chunk;
    (void)userdata;
    return 0;
}

static int dummy_chunk_end_callback(CURL *easy, void *chunk, void *userdata) {
    (void)easy;
    (void)chunk;
    (void)userdata;
    return 0;
}

static int dummy_fnmatch_callback(void *ptr, int match_case, int match, const char *pattern, const char *string) {
    (void)ptr;
    (void)match_case;
    (void)match;
    (void)pattern;
    (void)string;
    return 0;
}

static int dummy_hstsread_callback(void *userptr, char *buffer, size_t size) {
    (void)userptr;
    (void)buffer;
    (void)size;
    return 0;
}

static int dummy_hstswrite_callback(void *userptr, const char *line, size_t len) {
    (void)userptr;
    (void)line;
    (void)len;
    return 0;
}

static int dummy_ssh_key_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userptr) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)userptr;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userptr) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)userptr;
    return CURLKHSTAT_FINE;
}

static int dummy_interleave_callback(CURL *easy, curl_socket_t fd, int what, void *userptr) {
    (void)easy;
    (void)fd;
    (void)what;
    (void)userptr;
    return 0;
}

static int dummy_prereq_callback(CURL *easy, void *userptr) {
    (void)easy;
    (void)userptr;
    return 0;
}

static int dummy_trailer_callback(CURL *easy, char *buf, size_t size, void *userptr) {
    (void)easy;
    (void)buf;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
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

    // Create a dummy socket for testing
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd >= 0) {
        dummy_socket = fd;
    } else {
        dummy_socket = CURL_SOCKET_BAD;
    }

    // Set various options to exercise type checking macros
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_IOCTLFUNCTION, dummy_ioctl_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
    curl_easy_setopt(easy_handle, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);
    curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t s = (curl_socket_t)Data[0];

    // Fuzz curl_multi_socket_all
    curl_multi_socket_all(multi_handle, &running_handles);

    // Fuzz curl_multi_socket
    curl_multi_socket(multi_handle, s, &running_handles);

    // Fuzz curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    if (dummy_socket >= 0) {
        close(dummy_socket);
        dummy_socket = -1;
    }

    return 0;
}