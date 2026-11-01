// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURLcode res;
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    struct curl_certinfo certinfo;
    long http_code = 0;
    double total_time = 0.0;
    curl_off_t size_download = 0;

    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Use part of the input as URL, ensuring it's null-terminated
    char url_buf[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    // Ensure URL is valid (basic check to avoid empty strings)
    if (url_len == 0) {
        url_buf[0] = 'h'; url_buf[1] = 't'; url_buf[2] = 't'; url_buf[3] = 'p';
        url_buf[4] = ':'; url_buf[5] = '/'; url_buf[6] = '/'; url_buf[7] = 'e';
        url_buf[8] = 'x'; url_buf[9] = 'a'; url_buf[10] = 'm'; url_buf[11] = 'p';
        url_buf[12] = 'l'; url_buf[13] = '.'; url_buf[14] = 'c'; url_buf[15] = 'o';
        url_buf[16] = 'm'; url_buf[17] = '/'; url_buf[18] = 't'; url_buf[19] = 'e';
        url_buf[20] = 's'; url_buf[21] = 't'; url_buf[22] = '\0';
    }

    // Set basic options
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

    // Try to set POST data if input is large enough
    if (Size > 10) {
        char post_buf[1024];
        size_t post_len = (Size - 10 < 1023) ? (Size - 10) : 1023;
        memcpy(post_buf, Data + 10, post_len);
        post_buf[post_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_len);
    }

    // Try to create MIME structure and parts
    mime = curl_mime_init(curl);
    if (mime) {
        part = curl_mime_addpart(mime);
        if (part) {
            // Fuzz curl_mime_filename
            char filename_buf[128];
            size_t fn_len = (Size < 127) ? Size : 127;
            memcpy(filename_buf, Data, fn_len);
            filename_buf[fn_len] = '\0';
            curl_mime_filename(part, filename_buf);

            // Fuzz curl_mime_filedata with dummy file
            write_dummy_file("./dummy_file", Data, Size);
            curl_mime_filedata(part, "./dummy_file");
        }
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    }

    // Execute the transfer
    res = curl_easy_perform(curl);

    // Retrieve info after perform
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
        
        // Test CURLINFO_CERTINFO
        memset(&certinfo, 0, sizeof(certinfo));
        curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    }

    // Test curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}