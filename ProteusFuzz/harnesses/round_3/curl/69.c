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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type checking macros */
static int dummy_debug_callback(CURL *curl, curl_infotype type,
                                char *data, size_t size, void *userptr) {
    (void)curl; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_xferinfo_callback(CURL *curl, double dltotal, double dlnow,
                                   double ultotal, double ulnow, void *userptr) {
    (void)curl; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow; (void)userptr;
    return 0;
}

static int dummy_ssh_hostkey_callback(CURL *curl, int keytype,
                                      char *fingerprint, int fingerprint_len,
                                      char *rawkey, size_t rawlen,
                                      void *userptr) {
    (void)curl; (void)keytype; (void)fingerprint; (void)fingerprint_len;
    (void)rawkey; (void)rawlen; (void)userptr;
    return 0;
}

static int dummy_push_callback(CURLM *multi_handle, CURL *easy_handle,
                               int reason, void *userptr) {
    (void)multi_handle; (void)easy_handle; (void)reason; (void)userptr;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr; (void)size; (void)nmemb; (void)userptr;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr; (void)size; (void)nmemb; (void)userptr;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl; (void)ssl_ctx; (void)userptr;
    return 0;
}

static int dummy_conv_callback(char *buf, size_t len, void *userptr) {
    (void)buf; (void)len; (void)userptr;
    return 0;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

static int dummy_chunk_bgn_callback(CURL *curl, void *chunk, int files_in,
                                    void *userptr) {
    (void)curl; (void)chunk; (void)files_in; (void)userptr;
    return 0;
}

static int dummy_chunk_end_callback(CURL *curl, void *chunk, int files_in,
                                    int files_out, void *userptr) {
    (void)curl; (void)chunk; (void)files_in; (void)files_out; (void)userptr;
    return 0;
}

static int dummy_closesocket_callback(CURL *curl, curl_socket_t fd, void *userptr) {
    (void)curl; (void)fd; (void)userptr;
    return 0;
}

static int dummy_fnmatch_callback(const char *pattern, const char *string, void *userptr) {
    (void)pattern; (void)string; (void)userptr;
    return 0;
}

static int dummy_hstsread_callback(CURL *curl, const char *url, const char *host,
                                   const char *path, time_t now, time_t *age,
                                   void *userptr) {
    (void)curl; (void)url; (void)host; (void)path; (void)now; (void)age; (void)userptr;
    return 0;
}

static int dummy_hstswrite_callback(CURL *curl, const char *url, const char *host,
                                    const char *path, time_t now, time_t *age,
                                    void *userptr) {
    (void)curl; (void)url; (void)host; (void)path; (void)now; (void)age; (void)userptr;
    return 0;
}

static int dummy_sshkey_callback(CURL *curl, void *keyinfo, void *userptr) {
    (void)curl; (void)keyinfo; (void)userptr;
    return 0;
}

static int dummy_interleave_callback(CURL *curl, int what, void *userptr) {
    (void)curl; (void)what; (void)userptr;
    return 0;
}

static int dummy_prereq_callback(CURL *curl, void *userptr) {
    (void)curl; (void)userptr;
    return 0;
}

static int dummy_trailer_callback(CURL *curl, struct curl_slist **list, void *userptr) {
    (void)curl; (void)list; (void)userptr;
    return 0;
}

static int dummy_resolver_start_callback(CURL *curl, void *userptr) {
    (void)curl; (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Prepare dummy file for file-based options */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fprintf(fp, "dummy content for fuzzing\n");
        fclose(fp);
    }

    /* Extract values from input data to drive fuzzing */
    int opt_val = (Size > 0) ? Data[0] : 0;
    int long_val = (Size > 1) ? (int)Data[1] : 0;
    int off_t_val = (Size > 2) ? (int)Data[2] : 0;
    int error_code = (Size > 3) ? (int)Data[3] : 0;

    /* Fuzz curl_easy_setopt with various callbacks and options */
    /* Test xferinfo callback */
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void*)(size_t)opt_val);

    /* Test debug callback */
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)(size_t)opt_val);

    /* Test SSH hostkey callback */
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void*)(size_t)opt_val);

    /* Test SSL context callback */
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, (void*)(size_t)opt_val);

    /* Test conversion callbacks */
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);

    /* Test seek callback */
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void*)(size_t)opt_val);

    /* Test chunk callbacks */
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, (void*)(size_t)opt_val);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);

    /* Test close socket callback */
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, (void*)(size_t)opt_val);

    /* Test fnmatch callback */
    curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
    curl_easy_setopt(curl, CURLOPT_FNMATCH_DATA, (void*)(size_t)opt_val);

    /* Test HSTS callbacks */
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void*)(size_t)opt_val);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, (void*)(size_t)opt_val);

    /* Test SSH key callback */
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_sshkey_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, (void*)(size_t)opt_val);

    /* Test interleave callback */
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void*)(size_t)opt_val);

    /* Test prereq callback */
    curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
    curl_easy_setopt(curl, CURLOPT_PREREQDATA, (void*)(size_t)opt_val);

    /* Test trailer callback */
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
    curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void*)(size_t)opt_val);

    /* Test resolver start callback */
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_callback);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_DATA, (void*)(size_t)opt_val);

    /* Test write/read callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(size_t)opt_val);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void*)(size_t)opt_val);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)(size_t)opt_val);

    /* Test long options */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)opt_val);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)opt_val);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)long_val);

    /* Test off_t options */
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (curl_off_t)off_t_val);

    /* Test string options (using dummy file path or input data) */
    if (Size > 4) {
        char *url_str = (char*)malloc(Size - 4 + 1);
        if (url_str) {
            memcpy(url_str, Data + 4, Size - 4);
            url_str[Size - 4] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url_str);
            free(url_str);
        }
    }

    /* Test error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* Test file options */
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./dummy_file");

    /* Fuzz curl_multi_setopt */
    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
    curl_multi_setopt(multi, CURLMOPT_PUSHDATA, (void*)(size_t)opt_val);
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, (long)opt_val);
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)long_val);

    /* Fuzz curl_multi_strerror with various error codes */
    const char *err_str = curl_multi_strerror((CURLMcode)error_code);
    if (err_str) {
        /* Use the string to avoid unused variable warning */
        size_t len = strlen(err_str);
        (void)len;
    }

    /* Fuzz curl_multi_wakeup */
    CURLMcode wake_res = curl_multi_wakeup(multi);
    (void)wake_res;

    /* Fuzz curl_easy_reset */
    curl_easy_reset(curl);

    /* Cleanup */
    curl_multi_cleanup(multi);
    curl_easy_cleanup(curl);

    /* Clean up dummy file */
    remove("./dummy_file");

    return 0;
}