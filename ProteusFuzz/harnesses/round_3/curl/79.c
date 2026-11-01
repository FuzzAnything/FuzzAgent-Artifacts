// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
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
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

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

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_xferinfo_callback(CURL *handle, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)handle;
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
    return CURL_SOCKOPT_OK;
}

static int dummy_opensocket_callback(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp;
    (void)purpose;
    (void)address;
    return -1;
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

static int dummy_ioctrl_callback(CURL *handle, int cmd, void *clientp) {
    (void)handle;
    (void)cmd;
    (void)clientp;
    return CURLIOE_OK;
}

static int dummy_conv_callback(char *in, size_t inlen, char *out, size_t outlen, size_t *outwritten) {
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

static int dummy_chunk_bgn_callback(CURL *easy, void *chunk, void *ctx, int files_in, int files_out) {
    (void)easy;
    (void)chunk;
    (void)ctx;
    (void)files_in;
    (void)files_out;
    return CURL_CHUNK_BGN_FUNC_OK;
}

static int dummy_chunk_end_callback(CURL *easy, void *chunk, void *ctx, int files_in, int files_out) {
    (void)easy;
    (void)chunk;
    (void)ctx;
    (void)files_in;
    (void)files_out;
    return CURL_CHUNK_END_FUNC_OK;
}

static int dummy_fnmatch_callback(void *ptr, int match_case, int match, const char *pattern, const char *string) {
    (void)ptr;
    (void)match_case;
    (void)match;
    (void)pattern;
    (void)string;
    return 0;
}

static int dummy_hstsread_callback(void *ptr, const char *host, char *buf, size_t buflen) {
    (void)ptr;
    (void)host;
    (void)buf;
    (void)buflen;
    return 0;
}

static int dummy_hstswrite_callback(void *ptr, const char *host, const char *data) {
    (void)ptr;
    (void)host;
    (void)data;
    return 0;
}

static int dummy_ssh_key_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int keytype, void *userptr) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)keytype;
    (void)userptr;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int keytype, void *userptr) {
    (void)easy;
    (void)knownkey;
    (void)foundkey;
    (void)keytype;
    (void)userptr;
    return CURLKHSTAT_FINE;
}

static int dummy_interleave_callback(CURL *easy, curl_socket_t s, int what, void *userp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    return 0;
}

static int dummy_prereq_callback(CURL *easy, void *userp) {
    (void)easy;
    (void)userp;
    return 0;
}

static int dummy_trailer_callback(CURL *easy, char *data, size_t size, size_t nitems, void *userptr) {
    (void)easy;
    (void)data;
    (void)size;
    (void)nitems;
    (void)userptr;
    return 0;
}

static int dummy_resolver_start_callback(CURL *easy, void *resolver, void *userp) {
    (void)easy;
    (void)resolver;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
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

    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    int ev_bitmask = 0;

    // Extract values from fuzz input safely
    int option_idx = Data[0] % 100;
    int socket_fd = (int)Data[1];
    int bitmask = (int)Data[2];
    int long_val = (int)Data[3];

    // Create a dummy socket for testing
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock >= 0) {
        dummy_socket = sock;
    }

    // Fuzz curl_multi_assign
    if (dummy_socket >= 0 && Size > 4) {
        void *sockp = (void *)(uintptr_t)(Data[4] % 256);
        curl_multi_assign(multi_handle, dummy_socket, sockp);
    }

    // Fuzz curl_multi_socket (deprecated)
    if (dummy_socket >= 0) {
        curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    }

    // Fuzz curl_multi_socket_action
    if (dummy_socket >= 0) {
        curl_multi_socket_action(multi_handle, dummy_socket, bitmask, &running_handles);
    }

    // Fuzz curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Fuzz curl_easy_setopt with various options
    switch (option_idx) {
        case 0:
            if (Size > 0) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, (char *)Data);
            }
            break;
        case 1:
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            break;
        case 2:
            curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
            break;
        case 3:
            curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
            break;
        case 4:
            curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
            break;
        case 5:
            curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
            break;
        case 6:
            curl_easy_setopt(easy_handle, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
            break;
        case 7:
            curl_easy_setopt(easy_handle, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
            break;
        case 8:
            curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
            break;
        case 9:
            curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
            break;
        case 10:
            curl_easy_setopt(easy_handle, CURLOPT_IOCTLFUNCTION, dummy_ioctrl_callback);
            break;
        case 11:
            curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
            break;
        case 12:
            curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
            break;
        case 13:
            curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
            break;
        case 14:
            curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
            break;
        case 15:
            curl_easy_setopt(easy_handle, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
            break;
        case 16:
            curl_easy_setopt(easy_handle, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);
            break;
        case 17:
            curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
            break;
        case 18:
            curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
            break;
        case 19:
            curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
            break;
        case 20:
            curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
            break;
        case 21:
            curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
            break;
        case 22:
            curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
            break;
        case 23:
            curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
            break;
        case 24:
            curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
            break;
        case 25:
            curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_callback);
            break;
        case 26:
            if (Size > 5) {
                curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)(uintptr_t)(Data[5] % 256));
            }
            break;
        case 27:
            if (Size > 6) {
                curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)(uintptr_t)(Data[6] % 256));
            }
            break;
        case 28:
            if (Size > 0) {
                curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void *)Data);
            }
            break;
        case 29:
            if (Size > 7) {
                curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, (long)Data[7]);
            }
            break;
        default:
            break;
    }

    // Fuzz curl_multi_setopt
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, (curl_socket_callback)dummy_sockopt_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, (curl_multi_timer_callback)dummy_progress_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, long_val);

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    if (dummy_socket >= 0) {
        close(dummy_socket);
    }

    return 0;
}