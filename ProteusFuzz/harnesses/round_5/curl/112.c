// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
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
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl_multi_socket and related type-checking macros.
 * Note: curl_multi_socket is deprecated, but we fuzz it as requested.
 * The type-checking macros (Wcurl_*) are compile-time checks, so we
 * exercise them by calling curl_easy_setopt/getinfo with various types.
 */

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /*
     * Prepare dummy data from fuzz input to set options.
     * We use the input data to create strings, integers, etc.
     */
    int running_handles = 0;
    curl_socket_t sock = (curl_socket_t)(Data[0] & 0xFF);

    /*
     * Exercise curl_multi_socket with various socket values.
     * We also try to trigger recursive call detection by calling
     * curl_multi_socket_action (which is the recommended replacement)
     * and then curl_multi_socket.
     */
    CURLMcode rc = curl_multi_socket(multi_handle, sock, &running_handles);
    (void)rc; /* Ignore return value for fuzzing */

    /*
     * Exercise curl_multi_socket_action as well to explore more states.
     */
    rc = curl_multi_socket_action(multi_handle, sock, CURL_POLL_IN, &running_handles);
    (void)rc;

    /*
     * Set various options on easy_handle to exercise type-checking macros.
     * We use the fuzz data to construct arguments.
     */
    if (Size >= 1) {
        long opt_long = (long)(Data[0]);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, opt_long);
    }

    if (Size >= 2) {
        curl_off_t opt_off_t = (curl_off_t)(Data[0] | (Data[1] << 8));
        curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, (long)opt_off_t);
    }

    /*
     * Set string options using fuzz data.
     * Ensure null termination.
     */
    if (Size >= 1) {
        char str_buf[256];
        size_t str_len = (Size < 255) ? Size : 255;
        memcpy(str_buf, Data, str_len);
        str_buf[str_len] = '\0';

        curl_easy_setopt(easy_handle, CURLOPT_URL, str_buf);
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, str_buf);
        curl_easy_setopt(easy_handle, CURLOPT_REFERER, str_buf);
    }

    /*
     * Set callback options with dummy callbacks.
     * These exercise the type-checking macros for callbacks.
     */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, NULL);

    /*
     * Set slist options to exercise curlcheck_slist_option and related macros.
     */
    struct curl_slist *slist = NULL;
    if (Size >= 1) {
        char *list_str = (char *)malloc(Size + 1);
        if (list_str) {
            memcpy(list_str, Data, Size);
            list_str[Size] = '\0';
            slist = curl_slist_append(slist, list_str);
            free(list_str);
        }
    }
    if (slist) {
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(easy_handle, CURLOPT_QUOTE, slist);
        curl_slist_free_all(slist);
    }

    /*
     * Exercise curl_easy_getinfo with various types.
     */
    char *info_str = NULL;
    long info_long = 0;
    double info_double = 0.0;
    curl_off_t info_off_t = 0;
    curl_socket_t info_socket = CURL_SOCKET_BAD;
    struct curl_slist *info_slist = NULL;

    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &info_str);
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &info_long);
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &info_double);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &info_off_t);
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &info_socket);
    curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &info_slist);

    if (info_slist) {
        curl_slist_free_all(info_slist);
    }

    /*
     * Try to set options that trigger specific type-check warnings.
     * These are compile-time checks, but we ensure the code compiles
     * with correct types to avoid build errors.
     */
    curl_easy_setopt(easy_handle, CURLOPT_SSH_KEYFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_SSH_HOSTKEYFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_INTERLEAVEFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_PREREQFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_TRAILERFUNCTION, NULL);

    /*
     * Clean up.
     */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}