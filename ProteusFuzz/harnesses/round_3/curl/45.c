// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
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
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
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

static int dummy_seek_cb(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_FAIL;
}

static int dummy_ssh_key_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_cb(CURL *curl, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)curl; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_chunk_bgn_cb(CURL *easy, void *chunk, int dataleft) {
    (void)easy; (void)chunk; (void)dataleft;
    return 0;
}

static int dummy_chunk_end_cb(CURL *easy, void *chunk, int dataleft) {
    (void)easy; (void)chunk; (void)dataleft;
    return 0;
}

static int dummy_close_socket_cb(CURL *easy, curl_socket_t s, void *clientp) {
    (void)easy; (void)s; (void)clientp;
    return 0;
}

static int dummy_fnmatch_cb(void *ptr, const char *pattern, const char *string) {
    (void)ptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_hstsread_cb(void *clientp, const char *domain, char *buf, size_t buflen) {
    (void)clientp; (void)domain; (void)buf; (void)buflen;
    return 0;
}

static int dummy_hstswrite_cb(void *clientp, const char *domain, const char *hsts) {
    (void)clientp; (void)domain; (void)hsts;
    return 0;
}

static int dummy_interleave_cb(CURL *easy, int what, void *clientp) {
    (void)easy; (void)what; (void)clientp;
    return 0;
}

static int dummy_prereq_cb(CURL *easy, void *clientp) {
    (void)easy; (void)clientp;
    return 0;
}

static int dummy_trailer_cb(CURL *easy, char *data, size_t size, size_t nitems, void *userp) {
    (void)easy; (void)data; (void)size; (void)nitems; (void)userp;
    return 0;
}

static int dummy_xferinfo_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_ssl_ctx_cb(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl; (void)ssl_ctx; (void)userptr;
    return 0;
}

static int dummy_conv_cb(char *buf, size_t len, void *userp) {
    (void)buf; (void)len; (void)userp;
    return 0;
}

static int dummy_ioctlb(CURL *easy, int cmd, void *clientp) {
    (void)easy; (void)cmd; (void)clientp;
    return 0;
}

static int dummy_sockopt_cb(CURL *easy, curl_socket_t fd, curlsocktype purpose, void *userp) {
    (void)easy; (void)fd; (void)purpose; (void)userp;
    return 0;
}

static int dummy_resolver_start_cb(CURL *easy, void *clientp) {
    (void)easy; (void)clientp;
    return 0;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a share handle */
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    /* Allocate error buffer safely */
    char *error_buffer = malloc(CURL_ERROR_SIZE);
    if (!error_buffer) {
        curl_easy_cleanup(curl);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    memset(error_buffer, 0, CURL_ERROR_SIZE);

    /* Allocate a string buffer for URL and other string options */
    char *str_buf = malloc(Size + 1);
    if (!str_buf) {
        free(error_buffer);
        curl_easy_cleanup(curl);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    memcpy(str_buf, Data, Size);
    str_buf[Size] = '\0';

    /* Fuzz curl_share_setopt */
    /* Try various options */
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_PSL);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_HSTS);
    
    /* Try unshare */
    curl_share_setopt(share, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(share, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_DNS);
    
    /* Try invalid options if they exist in enum range, but we stick to known valid ones for stability */
    curl_share_setopt(share, CURLSHOPT_SHARE, (CURLSHoption)0);
    curl_share_setopt(share, CURLSHOPT_SHARE, (CURLSHoption)100);

    /* Associate share with curl handle */
    curl_easy_setopt(curl, CURLOPT_SHARE, share);

    /* Fuzz curl_easy_setopt with various options */
    
    /* String options */
    curl_easy_setopt(curl, CURLOPT_URL, str_buf);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
    curl_easy_setopt(curl, CURLOPT_REFERER, str_buf);
    curl_easy_setopt(curl, CURLOPT_COOKIE, str_buf);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CAPATH, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_SSLCERT, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_SSLKEY, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_PROXY, str_buf);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, str_buf);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, str_buf);
    curl_easy_setopt(curl, CURLOPT_RANGE, str_buf);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_buf);
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, str_buf);

    /* Long options */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_DNS_USE_GLOBAL_CACHE, 1L);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 60L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1L);
    curl_easy_setopt(curl, CURLOPT_UNRESTRICTED_AUTH, 1L);
    curl_easy_setopt(curl, CURLOPT_CRLF, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, NULL); /* struct curl_httppost */
    curl_easy_setopt(curl, CURLOPT_HTTP200ALIASES, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_QUOTE, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_PREQUOTE, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_RESOLVE, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_TELNETOPTIONS, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_CONNECT_TO, NULL); /* slist */
    curl_easy_setopt(curl, CURLOPT_PROXYHEADER, NULL); /* slist */

    /* Callback options */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_cb);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_cb);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_cb);
    curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_cb);
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_cb);
    curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
    curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, dummy_ioctlb);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_cb);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);

    /* Data pointers for callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_FNMATCH_DATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_PREREQDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_IOCTLDATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_DATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void*)0x1234);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)0x1234);

    /* Error buffer */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* Off_t options */
    curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, (curl_off_t)1024);
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM, (curl_off_t)0);
    curl_easy_setopt(curl, CURLOPT_MAXFILESIZE_LARGE, (curl_off_t)1024);
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, (curl_off_t)0);

    /* Curl handle options */
    curl_easy_setopt(curl, CURLOPT_STREAM_DEPENDS, NULL);
    curl_easy_setopt(curl, CURLOPT_STREAM_DEPENDS_E, NULL);

    /* Cleanup */
    free(str_buf);
    free(error_buffer);
    curl_easy_cleanup(curl);
    curl_share_cleanup(share);
    curl_global_cleanup();

    return 0;
}