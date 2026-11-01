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
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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

/* Dummy callback functions to satisfy type checks for various options */
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

static int dummy_opensocket_cb(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp; (void)purpose; (void)address;
    return -1;
}

static int dummy_seek_cb(void *clientp, curl_off_t offset, int origin) {
    (void)clientp; (void)offset; (void)origin;
    return 0;
}

static int dummy_conv_cb(void *clientp, char *in, size_t inlen, char *out, size_t outlen, size_t *outwritten) {
    (void)clientp; (void)in; (void)inlen; (void)out; (void)outlen; (void)outwritten;
    return 0;
}

static int dummy_hstswrite_cb(void *clientp, const char *host, const char *uri, time_t now, time_t age, const char *value) {
    (void)clientp; (void)host; (void)uri; (void)now; (void)age; (void)value;
    return 0;
}

static int dummy_hstsread_cb(void *clientp, const char *host, const char *uri, time_t *now, time_t *age, char *value, size_t vsize) {
    (void)clientp; (void)host; (void)uri; (void)now; (void)age; (void)value; (void)vsize;
    return 0;
}

static int dummy_trailer_cb(void *clientp, char *key, size_t klen, char *val, size_t vlen) {
    (void)clientp; (void)key; (void)klen; (void)val; (void)vlen;
    return 0;
}

static int dummy_sshkey_cb(void *clientp, const char *knownhost, const char *key, int keytype) {
    (void)clientp; (void)knownhost; (void)key; (void)keytype;
    return 0;
}

static int dummy_sshhostkey_cb(void *clientp, const char *host, int keytype, const char *key) {
    (void)clientp; (void)host; (void)keytype; (void)key;
    return 0;
}

static int dummy_interleave_cb(CURL *easy, curl_socket_t s, int what, void *clientp, void *socketp) {
    (void)easy; (void)s; (void)what; (void)clientp; (void)socketp;
    return 0;
}

static int dummy_prereq_cb(CURL *easy, void *clientp) {
    (void)easy; (void)clientp;
    return 0;
}

static int dummy_closesocket_cb(void *clientp, curl_socket_t curlfd) {
    (void)clientp; (void)curlfd;
    return 0;
}

static int dummy_fnmatch_cb(void *clientp, const char *pattern, const char *string) {
    (void)clientp; (void)pattern; (void)string;
    return 0;
}

static int dummy_chunk_bgn_cb(CURL *easy_handle, void *chunk, int files_in, int files_out, int bytes_in, int bytes_out) {
    (void)easy_handle; (void)chunk; (void)files_in; (void)files_out; (void)bytes_in; (void)bytes_out;
    return 0;
}

static int dummy_chunk_end_cb(CURL *easy_handle, void *chunk, int files_in, int files_out, int bytes_in, int bytes_out) {
    (void)easy_handle; (void)chunk; (void)files_in; (void)files_out; (void)bytes_in; (void)bytes_out;
    return 0;
}

static int dummy_ssl_ctx_cb(CURL *easy, void *ssl_ctx, void *userptr) {
    (void)easy; (void)ssl_ctx; (void)userptr;
    return 0;
}

static int dummy_resolver_start_cb(CURL *easy, void *clientp, void *resolverp) {
    (void)easy; (void)clientp; (void)resolverp;
    return 0;
}

static int dummy_sockopt_cb(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    (void)clientp; (void)curlfd; (void)purpose;
    return 0;
}

static int dummy_xferinfo_cb(CURL *easy, double dltotal, double dlnow, double ultotal, double ulnow, void *userp) {
    (void)easy; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
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

    /* Create a dummy file for FILE* options */
    FILE *dummy_file = fopen("./dummy_file", "w+");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        dummy_file = fopen("./dummy_file", "r");
    }

    /* Initialize variables for getinfo */
    char *str_ptr = NULL;
    long long_val = 0;
    double double_val = 0.0;
    struct curl_slist *slist_ptr = NULL;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    curl_off_t off_val = 0;

    /* Use input data to fuzz option values where appropriate */
    long fuzz_long = (long)(Data[0] * 100);
    curl_off_t fuzz_off = (curl_off_t)(Data[1] * 1000);
    int fuzz_int = (int)(Data[2] * 100);

    /* Fuzz various curl_easy_setopt calls with different argument types */
    
    /* Long options */
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, fuzz_long & 1);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, fuzz_long);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, fuzz_long);
    curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, fuzz_long);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, fuzz_long & 1);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, fuzz_long & 3);
    
    /* String options */
    if (Size > 10) {
        char *url_str = (char *)malloc(Size + 1);
        if (url_str) {
            memcpy(url_str, Data, Size);
            url_str[Size] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_URL, url_str);
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, url_str);
            curl_easy_setopt(easy_handle, CURLOPT_REFERER, url_str);
            curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, url_str);
            free(url_str);
        }
    }

    /* Post fields options */
    if (Size > 5) {
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (char *)Data);
        curl_easy_setopt(easy_handle, CURLOPT_COPYPOSTFIELDS, (char *)Data);
    }

    /* Callback options */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(easy_handle, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, dummy_sshkey_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_sshhostkey_cb);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_cb);
    curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_cb);
    curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_cb);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);
    curl_easy_setopt(easy_handle, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_cb);
    curl_easy_setopt(easy_handle, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);

    /* FILE* option */
    if (dummy_file) {
        curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)Size);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, dummy_file);
    }

    /* slist options */
    struct curl_slist *headers = NULL;
    if (Size > 20) {
        char header[256];
        snprintf(header, sizeof(header), "X-Fuzz: %s", Data);
        headers = curl_slist_append(headers, header);
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    }

    /* curl_multi_socket (deprecated but still tested) */
    curl_socket_t dummy_socket = -1;
    int running_handles = 0;
    curl_multi_socket(multi_handle, dummy_socket, &running_handles);

    /* curl_multi_perform */
    curl_multi_perform(multi_handle, &running_handles);

    /* Test curl_easy_getinfo with various types */
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &str_ptr);
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &double_val);
    curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &slist_ptr);
    curl_easy_getinfo(easy_handle, CURLINFO_TLS_SESSION, &tls_info);
    curl_easy_getinfo(easy_handle, CURLINFO_CERTINFO, &cert_info);
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &socket_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_UPLOAD_T, &off_val);

    /* Cleanup */
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (str_ptr) {
        /* Note: str_ptr is managed by curl, do not free */
    }
    if (slist_ptr) {
        curl_slist_free_all(slist_ptr);
    }
    if (dummy_file) {
        fclose(dummy_file);
    }

    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}