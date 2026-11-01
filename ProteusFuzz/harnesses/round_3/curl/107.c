// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback functions to satisfy type requirements */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_ssl_ctx_cb(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl; (void)ssl_ctx; (void)userptr;
    return 0;
}

static int dummy_conv_cb(char *in, size_t inlen, char *out, size_t outlen, size_t *outlenc) {
    (void)in; (void)inlen; (void)out; (void)outlen; (void)outlenc;
    return 0;
}

static int dummy_seek_cb(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

static int dummy_hstswrite_cb(void *userp, const char *host, const char *line) {
    (void)userp; (void)host; (void)line;
    return 0;
}

static int dummy_hstsread_cb(void *userp, const char *host, char *buf, size_t buflen) {
    (void)userp; (void)host; (void)buf; (void)buflen;
    return 0;
}

static int dummy_ssh_key_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userptr) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)userptr;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userptr) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)userptr;
    return CURLKHSTAT_FINE;
}

static int dummy_interleave_cb(CURL *easy, curl_socket_t s, int what, void *userp) {
    (void)easy; (void)s; (void)what; (void)userp;
    return 0;
}

static int dummy_prereq_cb(CURL *easy, void *prereq, void *userp) {
    (void)easy; (void)prereq; (void)userp;
    return 0;
}

static int dummy_trailer_cb(CURL *easy, char *buf, size_t size, void *userp) {
    (void)easy; (void)buf; (void)size; (void)userp;
    return 0;
}

static int dummy_close_socket_cb(CURL *easy, curl_socket_t s, void *userp) {
    (void)easy; (void)s; (void)userp;
    return 0;
}

static int dummy_fnmatch_cb(void *userp, const char *pattern, const char *string) {
    (void)userp; (void)pattern; (void)string;
    return 0;
}

static int dummy_chunk_bgn_cb(CURL *easy, void *userp) {
    (void)easy; (void)userp;
    return 0;
}

static int dummy_chunk_end_cb(CURL *easy, void *userp) {
    (void)easy; (void)userp;
    return 0;
}

static int dummy_opensocket_cb(CURL *easy, curlsocktype purpose, void *userp) {
    (void)easy; (void)purpose; (void)userp;
    return 0;
}

static int dummy_sockopt_cb(CURL *easy, curl_socket_t s, curlsocktype purpose, void *userp) {
    (void)easy; (void)s; (void)purpose; (void)userp;
    return 0;
}

static int dummy_xferinfo_cb(CURL *easy, double dltotal, double dlnow, double ultotal, double ulnow, void *userp) {
    (void)easy; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow; (void)userp;
    return 0;
}

static int dummy_resolver_start_cb(CURL *easy, void *resolver, void *userp) {
    (void)easy; (void)resolver; (void)userp;
    return 0;
}

static int dummy_push_cb(CURL *parent, CURL *easy, int flags, void *userp) {
    (void)parent; (void)easy; (void)flags; (void)userp;
    return CURL_PUSH_DENY;
}

/* Helper function to read a byte from the input buffer */
static uint8_t read_byte(const uint8_t **ptr, size_t *remaining) {
    if (*remaining > 0) {
        uint8_t val = **ptr;
        (*ptr)++;
        (*remaining)--;
        return val;
    }
    return 0;
}

/* Helper function to read an integer from the input buffer */
static int read_int(const uint8_t **ptr, size_t *remaining) {
    if (*remaining >= 4) {
        int val = (int)(((*ptr)[0] << 24) | ((*ptr)[1] << 16) | ((*ptr)[2] << 8) | (*ptr)[3]);
        *ptr += 4;
        *remaining -= 4;
        return val;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode mcode;
    CURLcode ecode;
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    /* Initialize curl global */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_multi_add_handle */
    if (read_byte(&ptr, &remaining) % 2 == 0) {
        mcode = curl_multi_add_handle(multi_handle, easy_handle);
        (void)mcode; /* Suppress unused warning */
    }

    /* Fuzz curl_multi_setopt */
    if (read_byte(&ptr, &remaining) % 3 == 0) {
        int option = read_int(&ptr, &remaining);
        /* Only fuzz known options to avoid undefined behavior */
        switch (option % 5) {
            case 0:
                mcode = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, read_int(&ptr, &remaining));
                break;
            case 1:
                mcode = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_cb);
                break;
            case 2:
                mcode = curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void*)Data);
                break;
            case 3:
                mcode = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, read_int(&ptr, &remaining));
                break;
            case 4:
                mcode = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, read_int(&ptr, &remaining));
                break;
        }
        (void)mcode;
    }

    /* Fuzz curl_easy_setopt with various options */
    if (read_byte(&ptr, &remaining) % 2 == 0) {
        int option = read_int(&ptr, &remaining);
        switch (option % 20) {
            case 0:
                /* Ensure the input data is null-terminated before passing to curl */
                if (Size > 0) {
                    /* Create a safe copy with null terminator */
                    char *safe_url = malloc(Size + 1);
                    if (safe_url) {
                        memcpy(safe_url, Data, Size);
                        safe_url[Size] = '\0';
                        ecode = curl_easy_setopt(easy_handle, CURLOPT_URL, safe_url);
                        free(safe_url);
                    }
                }
                break;
            case 1:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
                break;
            case 2:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void*)Data);
                break;
            case 3:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_cb);
                break;
            case 4:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void*)Data);
                break;
            case 5:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
                break;
            case 6:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
                break;
            case 7:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);
                break;
            case 8:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_cb);
                break;
            case 9:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_cb);
                break;
            case 10:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
                break;
            case 11:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
                break;
            case 12:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
                break;
            case 13:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_cb);
                break;
            case 14:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);
                break;
            case 15:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_cb);
                break;
            case 16:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
                break;
            case 17:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
                break;
            case 18:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_CLOSESOCKETFUNCTION, dummy_close_socket_cb);
                break;
            case 19:
                ecode = curl_easy_setopt(easy_handle, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_cb);
                break;
        }
        (void)ecode;
    }

    /* Fuzz curl_multi_wakeup */
    if (read_byte(&ptr, &remaining) % 4 == 0) {
        mcode = curl_multi_wakeup(multi_handle);
        (void)mcode;
    }

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    curl_global_cleanup();
    return 0;
}