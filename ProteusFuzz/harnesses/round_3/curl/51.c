// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* Forward declarations for types used in callbacks to avoid missing struct errors */
struct curl_hentry;

/* Dummy callback implementations */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return size * nmemb;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    (void)handle; (void)type; (void)data; (void)size; (void)userp;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_xferinfo_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    (void)clientp; (void)curlfd; (void)purpose;
    return 0;
}

static int dummy_opensocket_callback(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp; (void)purpose; (void)address;
    return -1;
}

static int dummy_closesocket_callback(void *clientp, curl_socket_t curlfd) {
    (void)clientp; (void)curlfd;
    return 0;
}

static int dummy_fnmatch_callback(void *clientp, const char *pattern, const char *string) {
    (void)clientp; (void)pattern; (void)string;
    return 0;
}

static int dummy_hstsread_callback(void *clientp, const char *domain, char *buffer, size_t buflen) {
    (void)clientp; (void)domain; (void)buffer; (void)buflen;
    return 0;
}

static int dummy_hstswrite_callback(void *clientp, const char *domain, const char *hsts) {
    (void)clientp; (void)domain; (void)hsts;
    return 0;
}

static int dummy_ssh_key_callback(void *clientp, const char *knownhost, const char *key, int keytype) {
    (void)clientp; (void)knownhost; (void)key; (void)keytype;
    return 0;
}

static int dummy_ssh_hostkey_callback(void *clientp, const char *host, int keytype, const char *key) {
    (void)clientp; (void)host; (void)keytype; (void)key;
    return 0;
}

static int dummy_interleave_callback(void *clientp, curl_socket_t fd, curl_socket_t *sockp) {
    (void)clientp; (void)fd; (void)sockp;
    return 0;
}

static int dummy_prereq_callback(void *clientp, curl_socket_t fd, curl_socket_t *sockp) {
    (void)clientp; (void)fd; (void)sockp;
    return 0;
}

static int dummy_trailer_callback(void *clientp, struct curl_slist *list) {
    (void)clientp; (void)list;
    return 0;
}

static int dummy_ssl_ctx_callback(void *clientp, void *ssl_ctx, void *ptr) {
    (void)clientp; (void)ssl_ctx; (void)ptr;
    return 0;
}

static int dummy_conv_callback(void *clientp, char *in, size_t inlen, char *out, size_t outlen, size_t *outlens) {
    (void)clientp; (void)in; (void)inlen; (void)out; (void)outlen; (void)outlens;
    return 0;
}

static int dummy_seek_callback(void *clientp, curl_off_t offset, int origin) {
    (void)clientp; (void)offset; (void)origin;
    return 0;
}

static int dummy_chunk_bgn_callback(void *clientp, void *chunk, int first, int *databuf) {
    (void)clientp; (void)chunk; (void)first; (void)databuf;
    return 0;
}

static int dummy_chunk_end_callback(void *clientp, void *chunk, int first) {
    (void)clientp; (void)chunk; (void)first;
    return 0;
}

static int dummy_resolver_start_callback(void *clientp, const char *hostname, int family, void *resolver) {
    (void)clientp; (void)hostname; (void)family; (void)resolver;
    return 0;
}

static int dummy_push_callback(CURL *parent, CURL *easy, size_t num_headers, struct curl_hentry *headers, void *userp) {
    (void)parent; (void)easy; (void)num_headers; (void)headers; (void)userp;
    return 0;
}

static int dummy_socket_callback(CURLM *multi, curl_socket_t s, int what, void *userp, void *sockp) {
    (void)multi; (void)s; (void)what; (void)userp; (void)sockp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

static int dummy_notify_callback(CURLM *multi, CURL *easy, CURLMSG msg, void *userp) {
    (void)multi; (void)easy; (void)msg; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Parse input to determine operations
    int op = Data[0] % 10;
    int socket_fd = (int)(Data[1] | (Data[2] << 8));
    int option = (int)(Data[3] | (Data[4] << 8));

    // Prepare dummy data for callbacks
    void *dummy_ptr = (void *)Data;
    char error_buf[CURL_ERROR_SIZE] = {0};

    // Perform operations based on input
    switch (op) {
        case 0:
            // curl_multi_assign
            curl_multi_assign(multi, socket_fd, dummy_ptr);
            break;
        case 1:
            // curl_multi_strerror
            {
                CURLMcode code = (CURLMcode)(Data[4] % 100);
                const char *str = curl_multi_strerror(code);
                if (str) {
                    (void)str[0]; // Use the string to avoid unused warning
                }
            }
            break;
        case 2:
            // curl_multi_remove_handle
            curl_multi_remove_handle(multi, easy);
            break;
        case 3:
            // curl_multi_wakeup
            curl_multi_wakeup(multi);
            break;
        case 4:
            // curl_multi_setopt with various options
            switch (option % 10) {
                case 0:
                    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                    break;
                case 1:
                    curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, dummy_ptr);
                    break;
                case 2:
                    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                    break;
                case 3:
                    curl_multi_setopt(multi, CURLMOPT_TIMERDATA, dummy_ptr);
                    break;
                case 4:
                    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
                    break;
                case 5:
                    curl_multi_setopt(multi, CURLMOPT_PUSHDATA, dummy_ptr);
                    break;
                case 6:
                    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)(Data[5] % 100));
                    break;
                case 7:
                    curl_multi_setopt(multi, CURLMOPT_PIPELINING, (long)(Data[6] % 2));
                    break;
                case 8:
                    curl_multi_setopt(multi, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Data[7] % 10));
                    break;
                case 9:
                    curl_multi_setopt(multi, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, (long)(Data[8] % 1000));
                    break;
            }
            break;
        case 5:
            // curl_easy_setopt with various options
            switch (option % 10) {
                case 0:
                    curl_easy_setopt(easy, CURLOPT_URL, (char *)Data);
                    break;
                case 1:
                    curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_callback);
                    break;
                case 2:
                    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_callback);
                    break;
                case 3:
                    curl_easy_setopt(easy, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
                    break;
                case 4:
                    curl_easy_setopt(easy, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
                    break;
                case 5:
                    curl_easy_setopt(easy, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
                    break;
                case 6:
                    curl_easy_setopt(easy, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
                    break;
                case 7:
                    curl_easy_setopt(easy, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
                    break;
                case 8:
                    curl_easy_setopt(easy, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
                    break;
                case 9:
                    curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, error_buf);
                    break;
            }
            break;
        case 6:
            // curl_easy_setopt with callback data
            curl_easy_setopt(easy, CURLOPT_READDATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_WRITEDATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_DEBUGDATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_PROGRESSDATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_XFERINFODATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_SOCKOPTDATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_OPENSOCKETDATA, dummy_ptr);
            curl_easy_setopt(easy, CURLOPT_CLOSESOCKETDATA, dummy_ptr);
            break;
        case 7:
            // curl_easy_setopt with string options
            curl_easy_setopt(easy, CURLOPT_CAINFO, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_CAPATH, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_COOKIE, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_COOKIEFILE, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_COOKIEJAR, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_CUSTOMREQUEST, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_INTERFACE, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_KEYPASSWD, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_PASSWORD, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_PROXY, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_PROXYUSERPWD, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_RANGE, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_REFERER, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_SSLCERT, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_SSLKEY, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_USERAGENT, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_USERNAME, (char *)Data);
            curl_easy_setopt(easy, CURLOPT_USERPWD, (char *)Data);
            break;
        case 8:
            // curl_easy_setopt with long options
            curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, (long)(Data[5] % 2));
            curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)(Data[6] % 100));
            curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, (long)(Data[7] % 100));
            curl_easy_setopt(easy, CURLOPT_MAXREDIRS, (long)(Data[8] % 10));
            curl_easy_setopt(easy, CURLOPT_SSL_VERIFYPEER, (long)(Data[9] % 2));
            curl_easy_setopt(easy, CURLOPT_SSL_VERIFYHOST, (long)(Data[10] % 2));
            curl_easy_setopt(easy, CURLOPT_VERBOSE, (long)(Data[11] % 2));
            curl_easy_setopt(easy, CURLOPT_HTTP_VERSION, (long)(Data[12] % 4));
            curl_easy_setopt(easy, CURLOPT_TCP_NODELAY, (long)(Data[13] % 2));
            curl_easy_setopt(easy, CURLOPT_KEEP_SENDING_ON_ERROR, (long)(Data[14] % 2));
            break;
        case 9:
            // curl_easy_setopt with curl_off_t options
            curl_easy_setopt(easy, CURLOPT_RESUME_FROM, (curl_off_t)(Data[5] | (Data[6] << 8)));
            curl_easy_setopt(easy, CURLOPT_MAXFILESIZE, (curl_off_t)(Data[7] | (Data[8] << 8)));
            curl_easy_setopt(easy, CURLOPT_MAXFILESIZE_LARGE, (curl_off_t)(Data[9] | (Data[10] << 8)));
            break;
    }

    // Cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}