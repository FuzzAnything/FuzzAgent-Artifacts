// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
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
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

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

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
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

static int dummy_ssh_key_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey,
                                  int khmatch, void *clientp) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)khmatch;
    (void)clientp;
    return CURLKHSTAT_FINE;
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

static int dummy_seek_callback(void *clientp, curl_off_t offset, int origin) {
    (void)clientp;
    (void)offset;
    (void)origin;
    return 0;
}

static int dummy_ioct_callback(curl_socket_t sockfd, int cmd, void *context) {
    (void)sockfd;
    (void)cmd;
    (void)context;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl;
    (void)ssl_ctx;
    (void)userptr;
    return 0;
}

static int dummy_conv_callback(void *clientp, char *in, size_t inlen, char *out, size_t outlen, size_t *outlens) {
    (void)clientp;
    (void)in;
    (void)inlen;
    (void)out;
    (void)outlen;
    (void)outlens;
    return 0;
}

static int dummy_fnmatch_callback(void *clientp, const char *pattern, const char *string) {
    (void)clientp;
    (void)pattern;
    (void)string;
    return 0;
}

static int dummy_trace_callback(void *clientp, const char *data, size_t size) {
    (void)clientp;
    (void)data;
    (void)size;
    return 0;
}

static int dummy_hstsread_callback(void *clientp, const char *url, char *buffer, size_t buflen) {
    (void)clientp;
    (void)url;
    (void)buffer;
    (void)buflen;
    return 0;
}

static int dummy_hstswrite_callback(void *clientp, const char *url, const char *hsts) {
    (void)clientp;
    (void)url;
    (void)hsts;
    return 0;
}

static int dummy_interleave_callback(CURL *easyhandle, int what, void *clientp) {
    (void)easyhandle;
    (void)what;
    (void)clientp;
    return 0;
}

static int dummy_prereq_callback(CURL *easy, void *clientp) {
    (void)easy;
    (void)clientp;
    return 0;
}

static int dummy_trailer_callback(CURL *easy, char *data, size_t size, size_t nitems, void *clientp) {
    (void)easy;
    (void)data;
    (void)size;
    (void)nitems;
    (void)clientp;
    return 0;
}

static int dummy_resolver_start_callback(void *resolver_start_data, curl_socket_t sockfd, int family, int type, int protocol,
                                         const char *hostname, int port, const struct curl_sockaddr *addr,
                                         void *clientp) {
    (void)resolver_start_data;
    (void)sockfd;
    (void)family;
    (void)type;
    (void)protocol;
    (void)hostname;
    (void)port;
    (void)addr;
    (void)clientp;
    return 0;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }

    // Initialize dummy file
    FILE *dummy_file = fopen("./dummy_file", "w+");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Reopen for reading
    dummy_file = fopen("./dummy_file", "r");

    // Set various options to explore different states
    curl_easy_setopt(easy_handle, CURLOPT_URL, (Size > 10) ? (char *)Data : "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SOCKOPTDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_OPENSOCKETDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SEEKDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_IOCTLFUNCTION, dummy_ioct_callback);
    curl_easy_setopt(easy_handle, CURLOPT_IOCTLDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_DATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
    curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_DATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_DATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSREADDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_PREREQDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFODATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_DATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYDATA, (void *)Data);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t sock = CURL_SOCKET_BAD;

    // Fuzz curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Fuzz curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Fuzz curl_multi_socket (deprecated)
    if (Size > 0) {
        sock = (curl_socket_t)(Data[0] & 0xFF);
        curl_multi_socket(multi_handle, sock, &running_handles);
    }

    // Fuzz curl_easy_getinfo with various types
    char *str_ptr = NULL;
    long long_val = 0;
    double double_val = 0.0;
    struct curl_slist *slist = NULL;
    struct curl_certinfo *certinfo = NULL;
    curl_socket_t socket_val = 0;
    curl_off_t off_t_val = 0;

    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &str_ptr);
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &double_val);
    curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &slist);
    curl_easy_getinfo(easy_handle, CURLINFO_CERTINFO, &certinfo);
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &socket_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD_T, &off_t_val);

    // Cleanup
    if (slist) {
        curl_slist_free_all(slist);
    }
    // Note: curl_certinfo_free does not exist in standard libcurl API.
    // The certinfo structure is managed internally or should not be freed manually by the user in this context.
    // We simply skip freeing it to avoid compilation errors.

    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);

    if (dummy_file) {
        fclose(dummy_file);
    }

    return 0;
}