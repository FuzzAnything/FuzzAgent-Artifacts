// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback functions to satisfy type checking if needed */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle; (void)type; (void)data; (void)size; (void)userdata;
    return 0;
}

static int dummy_sockopt_cb(CURL *handle, curl_socket_t fd, curlsocktype purpose) {
    (void)handle; (void)fd; (void)purpose;
    return 0;
}

static curl_socket_t dummy_opensocket_cb(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp; (void)purpose; (void)address;
    return CURL_SOCKET_BAD;
}

static int dummy_closesocket_cb(void *clientp, curl_socket_t fd) {
    (void)clientp; (void)fd;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_xferinfo_cb(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static CURLcode dummy_ssl_ctx_cb(CURL *curl, void *sslctx, void *userdata) {
    (void)curl; (void)sslctx; (void)userdata;
    return CURLSSLSET_OK;
}

static int dummy_conv_cb(void *clientp, char *in, size_t inlen, char *out, size_t outlen, size_t *outwritten) {
    (void)clientp; (void)in; (void)inlen; (void)out; (void)outlen; (void)outwritten;
    return 0;
}

static int dummy_seek_cb(void *clientp, curl_off_t offset, int origin) {
    (void)clientp; (void)offset; (void)origin;
    return 0;
}

static int dummy_chunk_bgn_cb(void *chunk, int dataindex, void *ptr) {
    (void)chunk; (void)dataindex; (void)ptr;
    return 0;
}

static int dummy_chunk_end_cb(void *chunk, void *ptr) {
    (void)chunk; (void)ptr;
    return 0;
}

static int dummy_fnmatch_cb(void *ptr, const char *pattern, const char *string) {
    (void)ptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_hstsread_cb(void *ptr, const char *domain, char *buf, size_t buflen) {
    (void)ptr; (void)domain; (void)buf; (void)buflen;
    return 0;
}

static int dummy_hstswrite_cb(void *ptr, const char *domain, const char *buf, size_t buflen) {
    (void)ptr; (void)domain; (void)buf; (void)buflen;
    return 0;
}

static int dummy_ssh_key_cb(void *keyinfo, const char *knownhosts, const char *fingerprint) {
    (void)keyinfo; (void)knownhosts; (void)fingerprint;
    return 0;
}

static int dummy_ssh_hostkey_cb(void *clientp, const char *knownhost, const char *knownhost_public, const char *fingerprint) {
    (void)clientp; (void)knownhost; (void)knownhost_public; (void)fingerprint;
    return 0;
}

static int dummy_resolver_start_cb(CURL *easy, void *resolver_start_data, void *userdata) {
    (void)easy; (void)resolver_start_data; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Prepare a dummy file if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Use data to drive fuzzing logic */
    unsigned int option_idx = 0;
    unsigned int action_idx = 0;
    unsigned int value_type = 0;

    if (Size >= 3) {
        option_idx = Data[0];
        action_idx = Data[1];
        value_type = Data[2];
    } else if (Size == 2) {
        option_idx = Data[0];
        action_idx = Data[1];
        value_type = 0;
    } else if (Size == 1) {
        option_idx = Data[0];
        action_idx = 0;
        value_type = 0;
    }

    /* Map option_idx to a valid CURLoption range to avoid undefined behavior */
    CURLoption option = (CURLoption)(option_idx % 300);
    
    /* Set some basic options to ensure the handle is somewhat configured */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* Fuzz curl_easy_setopt with various argument types based on value_type */
    switch (value_type % 10) {
        case 0: /* Long */
            {
                long val = 0;
                if (Size >= 2) {
                    val = (long)(Data[0] ^ Data[1]);
                }
                curl_easy_setopt(curl, option, val);
            }
            break;
        case 1: /* String (use input data as string, ensure null termination) */
            if (Size > 0) {
                char *str_buf = malloc(Size + 1);
                if (str_buf) {
                    memcpy(str_buf, Data, Size);
                    str_buf[Size] = '\0';
                    curl_easy_setopt(curl, option, str_buf);
                    free(str_buf);
                }
            }
            break;
        case 2: /* Callback: Write */
            curl_easy_setopt(curl, option, dummy_write_cb);
            break;
        case 3: /* Callback: Read */
            curl_easy_setopt(curl, option, dummy_read_cb);
            break;
        case 4: /* Callback: Debug */
            curl_easy_setopt(curl, option, dummy_debug_cb);
            break;
        case 5: /* Callback: Sockopt */
            curl_easy_setopt(curl, option, dummy_sockopt_cb);
            break;
        case 6: /* Callback: Opensocket */
            curl_easy_setopt(curl, option, dummy_opensocket_cb);
            break;
        case 7: /* Callback: Closesocket */
            curl_easy_setopt(curl, option, dummy_closesocket_cb);
            break;
        case 8: /* Callback: Progress */
            curl_easy_setopt(curl, option, dummy_progress_cb);
            break;
        case 9: /* Callback: Xferinfo */
            curl_easy_setopt(curl, option, dummy_xferinfo_cb);
            break;
    }

    /* Additional specific option fuzzing based on action_idx */
    if (action_idx % 5 == 0) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
    } else if (action_idx % 5 == 1) {
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
        curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    } else if (action_idx % 5 == 2) {
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)Data);
    } else if (action_idx % 5 == 3) {
        curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);
        curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, (void *)Data);
    } else {
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_cb);
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, (void *)Data);
    }

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Fuzz curl_easy_perform */
    curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo */
    long response_code = 0;
    curl_off_t size_download = 0;
    char *content_type = NULL;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    if (content_type) {
        /* content_type is owned by curl, do not free */
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    return 0;
}