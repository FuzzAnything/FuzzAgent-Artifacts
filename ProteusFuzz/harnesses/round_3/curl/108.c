// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
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
// curl_easy_perform at easy.c:817:10 in easy.h
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
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
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

/* 
 * The target functions (Wcurl_easy_getinfo_err_double, etc.) are compile-time
 * warning macros, not runtime functions. To fuzz the logic they guard, we must
 * invoke the actual runtime functions (curl_easy_getinfo, curl_easy_setopt)
 * with various inputs derived from the fuzz data.
 */

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Create a dummy file if needed for file-based options */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Prepare various data structures based on fuzz input */
    char *str_data = NULL;
    struct curl_slist *slist = NULL;
    struct curl_httppost *post = NULL;
    curl_mime *mime = NULL;
    char error_buf[CURL_ERROR_SIZE];
    double double_val;
    long long_val;
    curl_off_t off_val;
    curl_socket_t socket_val;
    struct curl_certinfo certinfo;
    struct curl_tlssessioninfo tlssession;
    char *str_ptr;

    /* Initialize variables */
    memset(error_buf, 0, CURL_ERROR_SIZE);
    memset(&certinfo, 0, sizeof(certinfo));
    memset(&tlssession, 0, sizeof(tlssession));
    double_val = 0.0;
    long_val = 0;
    off_val = 0;
    socket_val = CURL_SOCKET_BAD;

    /* Extract values from fuzz data */
    if (Size > 0) {
        long_val = (long)Data[0];
        off_val = (curl_off_t)Data[0];
        socket_val = (curl_socket_t)Data[0];
    }
    if (Size > 1) {
        double_val = (double)Data[1];
    }
    
    /* Create string from data (ensure null termination) */
    str_data = malloc(Size + 1);
    if (str_data) {
        memcpy(str_data, Data, Size);
        str_data[Size] = '\0';
        str_ptr = str_data;
    } else {
        str_ptr = NULL;
    }

    /* Create slist */
    if (str_data) {
        slist = curl_slist_append(slist, str_data);
        if (Size > 10) {
            slist = curl_slist_append(slist, "Header2: Value2");
        }
    }

    /* Create httppost */
    /* FIX: Pass &post as the second argument (last_post) instead of NULL.
       The API expects a pointer to the pointer that tracks the end of the list. */
    if (str_data) {
        curl_formadd(&post, &post,
                     CURLFORM_COPYNAME, "field1",
                     CURLFORM_COPYCONTENTS, str_data,
                     CURLFORM_END);
    }

    /* Create mime */
    mime = curl_mime_init(curl);
    if (mime && str_data) {
        curl_mimepart *part = curl_mime_addpart(mime);
        curl_mime_name(part, "file");
        curl_mime_data(part, str_data, (curl_off_t)Size);
    }

    /* Set options to trigger type checking logic */
    
    /* Test string options */
    if (str_data) {
        curl_easy_setopt(curl, CURLOPT_URL, str_data);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, str_data);
    }
    
    /* Test long options */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);

    /* Test slist options (triggers Wcurl_easy_setopt_err_curl_slist check) */
    if (slist) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_QUOTE, slist);
    }

    /* Test httppost options (triggers Wcurl_easy_setopt_err_curl_httpost check) */
    if (post) {
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    }

    /* Test mime options (triggers Wcurl_easy_setopt_err_curl_mimepost check) */
    if (mime) {
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    }

    /* Test error buffer option (triggers Wcurl_easy_setopt_err_error_buffer check) */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);

    /* Test callback options */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);

    /* Perform a dummy perform to ensure state is set */
    curl_easy_perform(curl);

    /* Test getinfo options with various types to trigger warning macros logic */
    
    /* Double info (triggers Wcurl_easy_getinfo_err_double) */
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &double_val);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_UPLOAD, &double_val);

    /* Long info (triggers Wcurl_easy_getinfo_err_long) */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &long_val);

    /* String info (triggers Wcurl_easy_getinfo_err_string) */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_ptr);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &str_ptr);

    /* Slist info (triggers Wcurl_easy_getinfo_err_curl_slist) */
    struct curl_slist *slist_out = NULL;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_out);
    if (slist_out) {
        curl_slist_free_all(slist_out);
    }

    /* Certinfo (triggers Wcurl_easy_getinfo_err_curl_certinfo) */
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);

    /* Socket info (triggers Wcurl_easy_getinfo_err_curl_socket) */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_val);

    /* Off_t info (triggers Wcurl_easy_getinfo_err_curl_off_t) */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);

    /* Cleanup */
    if (str_data) free(str_data);
    if (slist) curl_slist_free_all(slist);
    if (post) curl_formfree(post);
    if (mime) curl_mime_free(mime);

    curl_easy_cleanup(curl);

    return 0;
}