// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/options.h>
#include <curl/typecheck-gcc.h>

static void dummy_callback(void) {}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
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

static int dummy_sockopt_callback(void *clientp, curl_socket_t fd, curlsocktype purpose) {
    (void)clientp; (void)fd; (void)purpose;
    return CURL_SOCKOPT_OK;
}

static int dummy_opensocket_callback(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    (void)clientp; (void)purpose; (void)address;
    return -1;
}

static int dummy_closesocket_callback(void *clientp, curl_socket_t fd) {
    (void)clientp; (void)fd;
    return 0;
}

static int dummy_seek_callback(void *clientp, curl_off_t offset, int origin) {
    (void)clientp; (void)offset; (void)origin;
    return 0;
}

static int dummy_sshkeycallback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_sshhostkeycallback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *clientp) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)clientp;
    return CURLKHSTAT_FINE;
}

static int dummy_fnmatch_callback(void *ptr, const char *pattern, const char *string) {
    (void)ptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_interleave_callback(CURL *easy, int what, void *userp) {
    (void)easy; (void)what; (void)userp;
    return 0;
}

static int dummy_prereq_callback(CURL *easy, void *userp) {
    (void)easy; (void)userp;
    return 0;
}

static int dummy_trailer_callback(CURL *easy, char *data, size_t size, void *userp) {
    (void)easy; (void)data; (void)size; (void)userp;
    return 0;
}

static int dummy_hstsread_callback(void *userp, const char *domain, char *buf, size_t buflen) {
    (void)userp; (void)domain; (void)buf; (void)buflen;
    return 0;
}

static int dummy_hstswrite_callback(void *userp, const char *domain, const char *line) {
    (void)userp; (void)domain; (void)line;
    return 0;
}

static int dummy_resolver_start_callback(void *clientp, const char *hostname, int family, int flags, void **resolver_state, const curl_resolver_start_callback *resolver) {
    (void)clientp; (void)hostname; (void)family; (void)flags; (void)resolver_state; (void)resolver;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *easy, void *ssl_ctx, void *userp) {
    (void)easy; (void)ssl_ctx; (void)userp;
    return 0;
}

static int dummy_chunk_bgn_callback(CURL *easy, void *userdata, int chunk_tag, void **chunk_data) {
    (void)easy; (void)userdata; (void)chunk_tag; (void)chunk_data;
    return 0;
}

static int dummy_chunk_end_callback(CURL *easy, void *userdata, int chunk_tag, void **chunk_data) {
    (void)easy; (void)userdata; (void)chunk_tag; (void)chunk_data;
    return 0;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl */
    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Create a dummy file for file-based options */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Fuzz curl_version */
    char *version = curl_version();
    if (version) {
        /* Just read to ensure no crash */
        (void)version[0];
    }

    /* Fuzz curl_getenv */
    if (Size >= 2) {
        char var[256];
        size_t var_len = (Size > 255) ? 255 : Size - 1;
        memcpy(var, Data, var_len);
        var[var_len] = '\0';
        char *env_val = curl_getenv(var);
        if (env_val) {
            curl_free(env_val);
        }
    }

    /* Fuzz curl_easy_option_next */
    const struct curl_easyoption *opt = NULL;
    int iter_count = 0;
    while ((opt = curl_easy_option_next(opt)) != NULL) {
        iter_count++;
        if (iter_count > 10000) break; /* Safety limit */
    }

    /* Fuzz curl_easy_option_by_name */
    if (Size >= 2) {
        char name[256];
        size_t name_len = (Size > 255) ? 255 : Size - 1;
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        const struct curl_easyoption *opt_by_name = curl_easy_option_by_name(name);
        (void)opt_by_name;
    }

    /* Fuzz curl_easy_option_by_id */
    if (Size >= 2) {
        CURLoption id = (CURLoption)((Data[0] << 8) | Data[1]);
        const struct curl_easyoption *opt_by_id = curl_easy_option_by_id(id);
        (void)opt_by_id;
    }

    /* Fuzz curl_easy_setopt with various options */
    /* String options */
    if (Size >= 2) {
        char str_val[256];
        size_t str_len = (Size > 255) ? 255 : Size - 1;
        memcpy(str_val, Data, str_len);
        str_val[str_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, str_val);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, str_val);
        curl_easy_setopt(curl, CURLOPT_COOKIE, str_val);
        curl_easy_setopt(curl, CURLOPT_CAINFO, str_val);
    }

    /* Long options */
    if (Size >= 2) {
        long long_val = (long)((Data[0] << 8) | Data[1]);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, long_val);
    }

    /* Off_t options */
    if (Size >= 8) {
        curl_off_t off_val = 0;
        memcpy(&off_val, Data, 8);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)off_val);
    }

    /* Callback options */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_sshkeycallback);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_sshhostkeycallback);
    curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_callback);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);

    /* Data options for callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_FNMATCH_DATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_PREREQDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_DATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, (void*)Data);
    curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, (void*)Data);

    /* Post fields */
    if (Size >= 2) {
        /* Fix: Ensure null-termination for string options */
        char post_fields[Size + 1];
        memcpy(post_fields, Data, Size);
        post_fields[Size] = '\0';
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_fields);
    }

    /* Error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* File options */
    if (fp) {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_CAPATH, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_CRLFILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_NETRC_FILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_RANDOM_FILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSH_KNOWNHOSTS, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSH_PRIVATE_KEYFILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSH_PUBLIC_KEYFILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLCERT, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLKEY, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_ISSUERCERT, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_PROXY_CAINFO, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_PROXY_CAPATH, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_PROXY_CRLFILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_PROXY_ISSUERCERT, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_PROXY_SSLCERT, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_PROXY_SSLKEY, "./dummy_file");
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    remove("./dummy_file");

    return 0;
}