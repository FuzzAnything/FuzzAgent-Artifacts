// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_init at curl_share.c:33:9 in curl.h
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
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback implementations to satisfy type checking */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_xferinfo_callback(CURL *handle, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)handle; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_trailer_callback(CURL *handle, curl_off_t offset, curl_off_t size, void *userdata) {
    (void)handle; (void)offset; (void)size; (void)userdata;
    return 0;
}

static int dummy_conv_callback(char *in, size_t inlen, char *out, size_t outlen, size_t *outwritten) {
    (void)in; (void)inlen; (void)out; (void)outlen; (void)outwritten;
    return 0;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

static int dummy_ssh_key_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl; (void)ssl_ctx; (void)userptr;
    return 0;
}

static int dummy_hstsread_callback(void *userptr, char *buf, size_t size) {
    (void)userptr; (void)buf; (void)size;
    return 0;
}

static int dummy_hstswrite_callback(void *userptr, const char *buf, size_t size) {
    (void)userptr; (void)buf; (void)size;
    return 0;
}

static int dummy_fnmatch_callback(void *userptr, const char *pattern, const char *string) {
    (void)userptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_closesocket_callback(CURL *easy, curl_socket_t s, void *userptr) {
    (void)easy; (void)s; (void)userptr;
    return 0;
}

static int dummy_interleave_callback(CURL *easy, int what, void *userptr) {
    (void)easy; (void)what; (void)userptr;
    return 0;
}

static int dummy_prereq_callback(CURL *easy, void *userptr) {
    (void)easy; (void)userptr;
    return 0;
}

static int dummy_chunk_bgn_callback(CURL *easy, void *chunk, int num, void *userptr) {
    (void)easy; (void)chunk; (void)num; (void)userptr;
    return 0;
}

static int dummy_chunk_end_callback(CURL *easy, void *chunk, int num, void *userptr) {
    (void)easy; (void)chunk; (void)num; (void)userptr;
    return 0;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    CURLSH *share = curl_share_init();
    if (!share) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Prepare dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Extract option and data from fuzz input */
    int option = (int)(Data[0] % 200) + 1; /* Simulate various CURLoption values */
    const uint8_t *data_ptr = Data + 1;
    size_t data_len = Size - 1;

    /* Ensure null-termination for string options */
    char *null_terminated_data = NULL;
    if (data_len > 0) {
        null_terminated_data = malloc(data_len + 1);
        if (null_terminated_data) {
            memcpy(null_terminated_data, data_ptr, data_len);
            null_terminated_data[data_len] = '\0';
        }
    }

    /* Set various options to explore different code paths */
    switch (option % 10) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_URL, null_terminated_data ? null_terminated_data : "http://example.com");
            break;
        case 1:
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            curl_easy_setopt(curl, CURLOPT_READDATA, (void*)data_ptr);
            break;
        case 2:
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)data_ptr);
            break;
        case 3:
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
            curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)data_ptr);
            break;
        case 4:
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void*)data_ptr);
            break;
        case 5:
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void*)data_ptr);
            break;
        case 6:
            curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
            curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void*)data_ptr);
            break;
        case 7:
            curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
            curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
            curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
            break;
        case 8:
            curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
            curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void*)data_ptr);
            break;
        case 9:
            curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, (void*)data_ptr);
            curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void*)data_ptr);
            break;
    }

    /* Test curl_share_setopt */
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_PSL);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_HSTS);
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    if (share) {
        curl_share_cleanup(share);
    }
    free(null_terminated_data);

    return 0;
}