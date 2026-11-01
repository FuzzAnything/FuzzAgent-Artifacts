// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy file for file-based options if needed */
#define DUMMY_FILE "./dummy_file"

/* Helper to create a dummy file with fuzz data */
static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen(DUMMY_FILE, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

/* Dummy callback implementations to satisfy type checks if needed */
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

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    char *env_var = NULL;
    char *version = NULL;
    struct curl_tlssessioninfo tls_info;
    struct curl_certinfo cert_info;
    long long_info;
    double double_info;
    curl_off_t off_info;
    curl_socket_t sock_info;
    char *str_info;
    struct curl_slist *slist_info = NULL;

    /* Initialize curl */
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Create dummy file if data is available */
    if (Size > 0) {
        create_dummy_file(Data, Size);
    }

    /* Fuzz curl_easy_setopt with various options using fuzz data */
    if (Size > 0) {
        /* Long options */
        long long_val = (Size > 0) ? (long)Data[0] : 0;
        res = curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);
        (void)res;

        /* String options - ensure null termination */
        char *str_val = (char *)malloc(Size + 1);
        if (str_val) {
            memcpy(str_val, Data, Size);
            str_val[Size] = '\0';
            res = curl_easy_setopt(curl, CURLOPT_URL, str_val);
            (void)res;
            res = curl_easy_setopt(curl, CURLOPT_USERAGENT, str_val);
            (void)res;
            res = curl_easy_setopt(curl, CURLOPT_CAINFO, DUMMY_FILE);
            (void)res;
            res = curl_easy_setopt(curl, CURLOPT_COOKIEFILE, DUMMY_FILE);
            (void)res;
            free(str_val);
        }

        /* Callback options */
        res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
        (void)res;
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        (void)res;
        res = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        (void)res;

        /* POST fields */
        if (Size > 0) {
            res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (void *)Data);
            (void)res;
        }

        /* File pointer option */
        FILE *fp = fopen(DUMMY_FILE, "rb");
        if (fp) {
            res = curl_easy_setopt(curl, CURLOPT_INFILE, fp);
            (void)res;
            fclose(fp);
        }
    }

    /* Fuzz curl_getenv */
    if (Size > 0) {
        char *env_name = (char *)malloc(Size + 1);
        if (env_name) {
            memcpy(env_name, Data, Size);
            env_name[Size] = '\0';
            env_var = curl_getenv(env_name);
            free(env_name);
            if (env_var) {
                curl_free(env_var);
                env_var = NULL;
            }
        }
    }

    /* Fuzz curl_version */
    version = curl_version();
    (void)version; /* Just call it, no cleanup needed */

    /* Fuzz curl_easy_getinfo with various types */
    /* Long info */
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    (void)res;

    /* Double info */
    res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_info);
    (void)res;

    /* Off_t info */
    res = curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_info);
    (void)res;

    /* Socket info */
    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_info);
    (void)res;

    /* String info */
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &str_info);
    (void)res;

    /* Slist info */
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
    (void)res;
    if (slist_info) {
        curl_slist_free_all(slist_info);
        slist_info = NULL;
    }

    /* TLS session info (using the warning macro context) */
    memset(&tls_info, 0, sizeof(tls_info));
    res = curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
    (void)res;

    /* Cert info */
    memset(&cert_info, 0, sizeof(cert_info));
    res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
    (void)res;

    /* Fuzz curl_pushheader_bynum and curl_pushheader_byname */
    /* Note: struct curl_pushheaders is opaque. We cannot instantiate it directly.
       We pass a NULL pointer to test error handling.
       In a real scenario, this struct is populated by libcurl during a server push callback. */
    
    /* Test with NULL pointer */
    char *header_by_num_null = curl_pushheader_bynum(NULL, 0);
    (void)header_by_num_null; /* Should be NULL */

    char *header_by_name_null = curl_pushheader_byname(NULL, "Content-Type");
    (void)header_by_name_null; /* Should be NULL */

    /* Avoid passing a dangling or invalid pointer that causes ASAN stack-buffer-overflow.
       The previous attempt to cast an int to the struct caused the library to dereference
       invalid memory. We skip that test case to ensure stability. */

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}