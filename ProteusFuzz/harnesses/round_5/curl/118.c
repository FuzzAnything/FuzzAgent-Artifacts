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
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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
#include <stdio.h>

/* Dummy callback implementations to satisfy type checking */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)userdata;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle; (void)type; (void)data; (void)size; (void)userdata;
    return 0;
}

static int dummy_ssh_hostkey_cb(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userdata) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)userdata;
    return CURLKHSTAT_FINE;
}

static int dummy_socks5_gssapi_service_cb(void *userdata, char *service, char *host, size_t maxsize) {
    (void)userdata; (void)service; (void)host; (void)maxsize;
    return 0;
}

static int dummy_telnet_option_cb(CURL *easy, int cmd, int option, void *userdata) {
    (void)easy; (void)cmd; (void)option; (void)userdata;
    return 0;
}

static int dummy_progress_cb(void *userdata, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)userdata; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_xferinfo_cb(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)userdata; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_sockopt_cb(void *userdata, curl_socket_t fd, curlsocktype purpose) {
    (void)userdata; (void)fd; (void)purpose;
    return 0;
}

static int dummy_opensocket_cb(void *userdata, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)userdata; (void)purpose; (void)address;
    return -1;
}

static int dummy_closesocket_cb(void *userdata, curl_socket_t fd) {
    (void)userdata; (void)fd;
    return 0;
}

static int dummy_fnmatch_cb(void *userdata, const char *pattern, const char *string) {
    (void)userdata; (void)pattern; (void)string;
    return 0;
}

static int dummy_ioctlb(CURL *easy, int cmd, void *userdata) {
    (void)easy; (void)cmd; (void)userdata;
    return 0;
}

static int dummy_seek_cb(void *userdata, curl_off_t offset, int origin) {
    (void)userdata; (void)offset; (void)origin;
    return 0;
}

static int dummy_hstsread_cb(void *userdata, const char *domain, char *buffer, size_t buflen) {
    (void)userdata; (void)domain; (void)buffer; (void)buflen;
    return 0;
}

static int dummy_hstswrite_cb(void *userdata, const char *domain, const char *value, size_t len) {
    (void)userdata; (void)domain; (void)value; (void)len;
    return 0;
}

static int dummy_resolver_start_cb(void *userdata, const char *hostname, int family, int flags, void **resolver) {
    (void)userdata; (void)hostname; (void)family; (void)flags; (void)resolver;
    return 0;
}

static int dummy_trailer_cb(void *userdata, char *data, size_t size) {
    (void)userdata; (void)data; (void)size;
    return 0;
}

static int dummy_ssl_ctx_cb(void *userdata, CURL *easy, void *ssl_ctx) {
    (void)userdata; (void)easy; (void)ssl_ctx;
    return 0;
}

static int dummy_chunk_cb(void *userdata, char *ptr, size_t size) {
    (void)userdata; (void)ptr; (void)size;
    return 0;
}

static int dummy_ssh_key_cb(void *userdata, const char *key, size_t keylen, int keytype) {
    (void)userdata; (void)key; (void)keylen; (void)keytype;
    return 0;
}

static int dummy_prereq_cb(CURL *easy, void *userdata) {
    (void)easy; (void)userdata;
    return 0;
}

static int dummy_interleave_cb(CURL *easy, int read, int write, int error, void *userdata) {
    (void)easy; (void)read; (void)write; (void)error; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));

    /* Initialize variables for getinfo */
    long long_val = 0;
    double double_val = 0.0;
    char *string_val = NULL;
    curl_off_t off_val = 0;
    curl_socket_t sock_val = CURL_SOCKET_BAD;
    struct curl_tlssessioninfo tls_info;
    struct curl_certinfo cert_info;
    struct curl_slist *slist_val = NULL;
    struct curl_slist *slist_info = NULL;

    /* Use input data to drive fuzzing */
    int option_type = Data[0] % 10;
    int info_type = (Data[0] + 1) % 10;
    int use_fuzz_data = (Data[0] & 1);
    size_t offset = 1;

    /* Helper to get string from fuzz data */
    static char fuzz_str[256];
    size_t str_len = (Size > offset + 255) ? 255 : (Size - offset);
    if (str_len > 0) {
        memcpy(fuzz_str, &Data[offset], str_len);
        fuzz_str[str_len] = '\0';
        offset += str_len;
    } else {
        fuzz_str[0] = '\0';
    }

    /* Set various options based on input */
    switch (option_type) {
        case 0: /* CURLOPT_URL */
            if (use_fuzz_data) {
                curl_easy_setopt(curl, CURLOPT_URL, fuzz_str);
            } else {
                curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            }
            break;
        case 1: /* CURLOPT_POSTFIELDS */
            if (use_fuzz_data) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fuzz_str);
            } else {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "test");
            }
            break;
        case 2: /* CURLOPT_WRITEFUNCTION */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)0x1234);
            break;
        case 3: /* CURLOPT_READFUNCTION */
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
            curl_easy_setopt(curl, CURLOPT_READDATA, (void*)0x1234);
            break;
        case 4: /* CURLOPT_HTTPHEADER */
            if (use_fuzz_data) {
                struct curl_slist *list = curl_slist_append(NULL, fuzz_str);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                curl_slist_free_all(list);
            } else {
                struct curl_slist *list = curl_slist_append(NULL, "Host: example.com");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                curl_slist_free_all(list);
            }
            break;
        case 5: /* CURLOPT_SSH_HOSTKEYFUNCTION */
            curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);
            curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void*)0x1234);
            break;
        case 6: /* CURLOPT_DEBUGFUNCTION */
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
            curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)0x1234);
            break;
        case 7: /* CURLOPT_PROGRESSFUNCTION */
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void*)0x1234);
            break;
        case 8: /* CURLOPT_XFERINFOFUNCTION */
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_cb);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void*)0x1234);
            break;
        case 9: /* CURLOPT_ERRORBUFFER */
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
            break;
    }

    /* Perform a dummy perform to initialize state if needed, but we don't need a real network call */
    /* curl_easy_perform(curl); */

    /* Get various info based on input */
    switch (info_type) {
        case 0: /* CURLINFO_EFFECTIVE_URL */
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_val);
            break;
        case 1: /* CURLINFO_RESPONSE_CODE */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
            break;
        case 2: /* CURLINFO_TOTAL_TIME */
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_val);
            break;
        case 3: /* CURLINFO_SIZE_DOWNLOAD_T */
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
            break;
        case 4: /* CURLINFO_ACTIVESOCKET */
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_val);
            break;
        case 5: /* CURLINFO_TLS_SESSION */
            memset(&tls_info, 0, sizeof(tls_info));
            curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
            break;
        case 6: /* CURLINFO_CERTINFO */
            memset(&cert_info, 0, sizeof(cert_info));
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
            break;
        case 7: /* CURLINFO_COOKIELIST */
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
            if (slist_info) {
                curl_slist_free_all(slist_info);
            }
            break;
        case 8: /* CURLINFO_SSL_ENGINES */
            curl_easy_getinfo(curl, CURLINFO_SSL_ENGINES, &slist_info);
            if (slist_info) {
                curl_slist_free_all(slist_info);
            }
            break;
        case 9: /* CURLINFO_PRIVATE */
            curl_easy_getinfo(curl, CURLINFO_PRIVATE, &string_val);
            break;
    }

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}