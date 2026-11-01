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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static curl_off_t dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_FAIL;
}

static void dummy_free_callback(void *ptr) {
    (void)ptr;
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

static int dummy_sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    (void)clientp; (void)curlfd; (void)purpose;
    return CURL_SOCKOPT_OK;
}

static curl_socket_t dummy_opensocket_callback(void *clientp, curlsocktype purpose) {
    (void)clientp; (void)purpose;
    return CURL_SOCKET_BAD;
}

static int dummy_closesocket_callback(void *clientp, curl_socket_t curlfd) {
    (void)clientp; (void)curlfd;
    return 0;
}

static int dummy_xferinfo_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_ssl_ctx_callback(CURL *curl, void *sslctx, void *userptr) {
    (void)curl; (void)sslctx; (void)userptr;
    return 0;
}

static char *dummy_conv_callback(char *ptr, size_t inlen, size_t *outlen, void *userdata) {
    (void)ptr; (void)inlen; (void)outlen; (void)userdata;
    return NULL;
}

static int dummy_seek_callback_file(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

static int dummy_chunk_bgn_callback(CURL *easy, void *chunk, int files_in, int files_out) {
    (void)easy; (void)chunk; (void)files_in; (void)files_out;
    return CURL_CHUNK_BGN_FUNC_OK;
}

static int dummy_chunk_end_callback(CURL *easy, void *chunk, int files_in, int files_out) {
    (void)easy; (void)chunk; (void)files_in; (void)files_out;
    return CURL_CHUNK_END_FUNC_OK;
}

static int dummy_fnmatch_callback(void *ptr, const char *pattern, const char *string) {
    (void)ptr; (void)pattern; (void)string;
    return 0;
}

static int dummy_hstsread_callback(void *userptr, const char *domain, char *buf, size_t buflen) {
    (void)userptr; (void)domain; (void)buf; (void)buflen;
    return 0;
}

static int dummy_hstswrite_callback(void *userptr, const char *domain, const char *buf, size_t buflen) {
    (void)userptr; (void)domain; (void)buf; (void)buflen;
    return 0;
}

static int dummy_ssh_key_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userdata) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)userdata;
    return CURLKHSTAT_FINE;
}

static int dummy_ssh_hostkey_callback(CURL *easy, const struct curl_khkey *knownkey, const struct curl_khkey *foundkey, int khmatch, void *userdata) {
    (void)easy; (void)knownkey; (void)foundkey; (void)khmatch; (void)userdata;
    return CURLKHSTAT_FINE;
}

static int dummy_interleave_callback(CURL *easy, int what, void *clientp) {
    (void)easy; (void)what; (void)clientp;
    return 0;
}

static int dummy_prereq_callback(CURL *easy, void *clientp) {
    (void)easy; (void)clientp;
    return 0;
}

static int dummy_trailer_callback(CURL *easy, void *clientp) {
    (void)easy; (void)clientp;
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize dummy file for file-based options if needed */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Fuzz curl_easy_setopt with various options and types */
    if (Size > 0) {
        long long_val = (long)Data[0];
        curl_off_t off_val = (curl_off_t)Data[0];
        void *cb_data = (void *)Data;

        /* Test long options */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, long_val);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);

        /* Test off_t options */
        curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, off_val);

        /* Test string options: Ensure null-termination */
        /* We create a local buffer to safely hold the string */
        char *str_buf = malloc(Size + 1);
        if (str_buf) {
            memcpy(str_buf, Data, Size);
            str_buf[Size] = '\0'; /* Ensure null termination */

            curl_easy_setopt(curl, CURLOPT_URL, str_buf);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, str_buf);
            curl_easy_setopt(curl, CURLOPT_RANGE, str_buf);
            
            free(str_buf);
        }

        /* Test callback options */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
        curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_callback);
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, dummy_opensocket_callback);
        curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, dummy_closesocket_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dummy_xferinfo_callback);
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_callback);
        curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
        curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
        curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
        curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback_file);
        curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
        curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_callback);
        curl_easy_setopt(curl, CURLOPT_FNMATCH_FUNCTION, dummy_fnmatch_callback);
        curl_easy_setopt(curl, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_callback);
        curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_callback);
        curl_easy_setopt(curl, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
        curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);
        curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
        curl_easy_setopt(curl, CURLOPT_PREREQFUNCTION, dummy_prereq_callback);
        curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_callback);

        /* Test callback data options */
        curl_easy_setopt(curl, CURLOPT_READDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_SEEKDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_FNMATCH_DATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_HSTSREADDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_SSH_KEYDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_PREREQDATA, cb_data);
        curl_easy_setopt(curl, CURLOPT_TRAILERDATA, cb_data);
    }

    /* Fuzz curl_mime_init and curl_mime_free */
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        /* Add a part to the MIME structure */
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Ensure data is null-terminated for string fields if needed, 
               but curl_mime_data handles raw bytes correctly. 
               We pass Size to indicate length. */
            curl_mime_data(part, (char *)Data, Size);
            
            /* Ensure name and filename are null-terminated */
            char *name_buf = malloc(Size + 1);
            if (name_buf) {
                memcpy(name_buf, Data, Size);
                name_buf[Size] = '\0';
                curl_mime_name(part, name_buf);
                curl_mime_filename(part, name_buf);
                free(name_buf);
            }
        }

        /* Free the MIME structure */
        curl_mime_free(mime);
    }

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}