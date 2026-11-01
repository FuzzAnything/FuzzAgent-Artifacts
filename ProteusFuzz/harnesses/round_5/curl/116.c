// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type checks */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_OK;
}

static int dummy_hstsread_callback(char *buffer, size_t size, void *userdata) {
    (void)buffer; (void)size; (void)userdata;
    return 0;
}

static int dummy_hstswrite_callback(const char *buffer, size_t size, void *userdata) {
    (void)buffer; (void)size; (void)userdata;
    return 0;
}

static int dummy_ssh_key_callback(struct curl_khkey *key, int keytype, void *userdata) {
    (void)key; (void)keytype; (void)userdata;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_callback(const char *host, int port, const char *key, size_t keylen, void *userdata) {
    (void)host; (void)port; (void)key; (void)keylen; (void)userdata;
    return CURLKHMATCH_OK;
}

static int dummy_interleave_callback(CURL *easy_handle, curl_socket_t s, int what, void *userdata) {
    (void)easy_handle; (void)s; (void)what; (void)userdata;
    return 0;
}

static int dummy_prereq_callback(CURL *easy_handle, void *userdata) {
    (void)easy_handle; (void)userdata;
    return 0;
}

static int dummy_trailer_callback(CURL *easy_handle, char *data, size_t size, void *userdata) {
    (void)easy_handle; (void)data; (void)size; (void)userdata;
    return 0;
}

static int dummy_fnmatch_callback(void *userdata, const char *pattern, const char *string) {
    (void)userdata; (void)pattern; (void)string;
    return 0;
}

static int dummy_close_socket_callback(CURL *easy_handle, curl_socket_t s, void *userdata) {
    (void)easy_handle; (void)s; (void)userdata;
    return 0;
}

static int dummy_chunk_bgn_callback(void *userdata, int flags, void *chunk) {
    (void)userdata; (void)flags; (void)chunk;
    return 0;
}

static int dummy_chunk_end_callback(void *userdata, void *chunk) {
    (void)userdata; (void)chunk;
    return 0;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set various options to exercise type checking macros */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
    curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(easy_handle, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Exercise curl_multi_perform */
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    /* Exercise deprecated curl_multi_socket */
    curl_socket_t dummy_socket = -1;
    curl_multi_socket(multi_handle, dummy_socket, &running_handles);

    /* Exercise deprecated curl_multi_socket_all */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* Exercise curl_easy_getinfo with various types */
    double d_val;
    long l_val;
    char *s_val = NULL;
    curl_off_t o_val;
    curl_socket_t sock_val;
    struct curl_slist *slist_val = NULL;
    struct curl_certinfo *certinfo_val = NULL;
    struct curl_tlssessioninfo *tlssessioninfo_val = NULL;

    curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_TYPE, &s_val);
    curl_easy_getinfo(easy_handle, CURLINFO_HTTP_CODE, &l_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &d_val);
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &d_val);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_UPLOAD, &o_val);
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &sock_val);
    curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &slist_val);
    curl_easy_getinfo(easy_handle, CURLINFO_CERTINFO, &certinfo_val);
    curl_easy_getinfo(easy_handle, CURLINFO_TLS_SESSION, &tlssessioninfo_val);

    /* Cleanup */
    if (slist_val) curl_slist_free_all(slist_val);
    /* Note: curl_certinfo and curl_tlssessioninfo are freed internally by curl_easy_cleanup 
       or require specific internal cleanup not exposed in standard API for manual free 
       in this context to avoid compilation errors with missing symbols. 
       We skip manual free for these specific types to ensure compilation. */

    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}