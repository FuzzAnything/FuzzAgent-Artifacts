// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_free at mime.c:1082:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* 
 * The target functions (Wcurl_*) are compile-time warning macros, not runtime functions.
 * To fuzz the logic they enforce, we must invoke curl_easy_setopt and curl_easy_getinfo
 * with various argument types derived from the fuzzer input.
 * The macros will trigger warnings at compile time if types are wrong, but at runtime
 * we test the robustness of curl against unexpected or malformed inputs.
 */

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

static int dummy_ssh_hostkey_callback(CURL *easy, const char *host, const char *fingerprint, int keytype, void *userptr) {
    (void)easy;
    (void)host;
    (void)fingerprint;
    (void)keytype;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Prepare dummy file if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Use input data to drive various curl options and getinfo calls */
    long opt_long = (long)Data[0];
    double opt_double = (double)Data[0];
    curl_off_t opt_off_t = (curl_off_t)Data[0];
    curl_socket_t opt_socket = (curl_socket_t)Data[0];

    /* Fuzz string options */
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf) {
        memcpy(str_buf, Data, Size);
        str_buf[Size] = '\0';

        /* Test string options */
        curl_easy_setopt(curl, CURLOPT_URL, str_buf);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
        curl_easy_setopt(curl, CURLOPT_REFERER, str_buf);
        curl_easy_setopt(curl, CURLOPT_COOKIE, str_buf);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_buf);

        /* Test long options */
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, opt_long);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, opt_long);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, opt_long);

        /* Test double options */
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, opt_double);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, opt_double);

        /* Test off_t options */
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM, opt_off_t);
        curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void *)Data);

        /* Test slist options */
        struct curl_slist *slist = NULL;
        slist = curl_slist_append(slist, str_buf);
        slist = curl_slist_append(slist, "Header2: value2");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_HTTP200ALIASES, slist);

        /* Test callback options */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        curl_easy_setopt(curl, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_callback);

        /* Test MIME post (if supported) */
        curl_mime *mime = curl_mime_init(curl);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            curl_mime_name(part, "field");
            curl_mime_data(part, str_buf, Size);
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
            curl_mime_free(mime);
        }

        /* Test getinfo with various types */
        char *info_str = NULL;
        long info_long = 0;
        double info_double = 0.0;
        curl_off_t info_off_t = 0;
        curl_socket_t info_socket = CURL_SOCKET_BAD;
        struct curl_slist *info_slist = NULL;
        struct curl_tlssessioninfo *info_tls = NULL;
        struct curl_certinfo *info_cert = NULL;

        /* String info */
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &info_str);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &info_str);

        /* Long info */
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info_long);
        curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &info_long);

        /* Double info */
        curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &info_double);
        curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &info_double);

        /* Off_t info */
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &info_off_t);
        curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &info_off_t);

        /* Socket info */
        curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &info_socket);

        /* Slist info */
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &info_slist);
        if (info_slist) curl_slist_free_all(info_slist);

        /* TLS session info */
        curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &info_tls);
        curl_easy_getinfo(curl, CURLINFO_TLS_SSL_PTR, &info_tls);

        /* Cert info */
        curl_easy_getinfo(curl, CURLINFO_CERTINFO, &info_cert);

        free(str_buf);
    }

    /* Perform a dummy perform to exercise internal state */
    curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}