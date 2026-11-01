// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/* Callback for curl_easy_ssls_export matching the expected signature */
static CURLcode ssl_export_callback(CURL *handle, void *userptr,
                                    const char *sessionid,
                                    const unsigned char *sessionid_data,
                                    unsigned long sessionid_len,
                                    const unsigned char *session,
                                    unsigned long session_len,
                                    long version, int type,
                                    const char *cipher,
                                    unsigned long cipher_len) {
    /* Do nothing, just consume the callback */
    return CURLE_OK;
}

/* Write callback for curl_easy_perform to avoid memory issues */
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userptr) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    CURL *handle = NULL;
    CURLcode res;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    struct curl_slist *headers = NULL;
    struct curl_certinfo certinfo;
    long http_code;
    double total_time;
    curl_off_t download_size;

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Basic setup to prevent immediate failures */
    curl_easy_setopt(handle, CURLOPT_URL, "data:text/plain,");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 100L);

    /* Create a dummy file for MIME file tests */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    /* 1. Test curl_easy_ssls_export */
    res = curl_easy_ssls_export(handle, ssl_export_callback, NULL);
    (void)res;

    /* 2. Test curl_easy_perform */
    res = curl_easy_perform(handle);
    (void)res;

    /* 3. Test curl_easy_pause */
    res = curl_easy_pause(handle, CURLPAUSE_RECV | CURLPAUSE_SEND);
    (void)res;
    res = curl_easy_pause(handle, CURLPAUSE_CONT);
    (void)res;

    /* 4. Test curl_easy_getinfo */
    memset(&certinfo, 0, sizeof(certinfo));
    res = curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo);
    (void)res;

    res = curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &http_code);
    (void)res;

    res = curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);
    (void)res;

    res = curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &download_size);
    (void)res;

    /* 5. Test curl_mime_headers */
    mime = curl_mime_init(handle);
    if (mime) {
        part = curl_mime_addpart(mime);
        if (part) {
            /* Create a simple header list if data is available */
            if (Size > 0) {
                /* Allocate buffer with space for null terminator */
                char *header_str = (char *)malloc(Size + 1);
                if (header_str) {
                    memcpy(header_str, Data, Size);
                    header_str[Size] = '\0';
                    
                    /* Ensure it looks like a header by adding a colon if missing */
                    if (strchr(header_str, ':') == NULL) {
                        size_t len = strlen(header_str);
                        if (len > 0) {
                            header_str[len - 1] = ':';
                            header_str[len] = '\0';
                        }
                    }
                    
                    /* curl_slist_append takes ownership of the string if we pass it, 
                       but we need to be careful. Actually, curl_slist_append copies the string.
                       However, to be safe and avoid leaks if the append fails or logic changes,
                       we let curl_slist_append handle the memory. 
                       Note: curl_slist_append copies the string, so we must free header_str 
                       if we don't pass it, OR if we pass it, curl_slist_free_all will free the copy.
                       Wait, curl_slist_append documentation says: "The string is copied."
                       So we MUST free header_str here to avoid the leak reported.
                       But we need to pass it to curl_slist_append.
                       Let's re-verify: curl_slist_append(char *data) -> copies data.
                       So we free header_str immediately after appending.
                    */
                    headers = curl_slist_append(headers, header_str);
                    free(header_str); /* Free the original buffer as curl copied it */
                }
            }

            /* Set headers on the part with take_ownership = 1 */
            /* This transfers ownership of the list to libcurl. 
               We must not free 'headers' manually afterwards. */
            res = curl_mime_headers(part, headers, 1);
            (void)res;

            /* Try setting data from file */
            curl_mime_filedata(part, "./dummy_file");

            /* Try setting data from callback */
            curl_mime_data(part, (char *)Data, Size);
        }
    }

    /* 6. Test curl_easy_upkeep */
    res = curl_easy_upkeep(handle);
    (void)res;

    /* Cleanup */
    if (mime) {
        curl_mime_free(mime);
    }
    /* Do NOT call curl_slist_free_all(headers) here because ownership was transferred 
       to the mime part when take_ownership was set to 1. */
    
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}