// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        const char *content = "Dummy content for fuzzing";
        fwrite(content, 1, strlen(content), fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Ensure dummy file exists for file-based operations */
    create_dummy_file();

    /* 1. Test curl_global_trace */
    /* Use input data as config string, ensuring null termination */
    char *config_str = (char *)malloc(Size + 1);
    if (config_str) {
        memcpy(config_str, Data, Size);
        config_str[Size] = '\0';
        curl_global_trace(config_str);
        free(config_str);
    }

    /* 2. Test curl_mime_filename */
    curl_mime *mime = curl_mime_init(NULL);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Fuzz filename from input data */
            char *filename = (char *)malloc(Size + 1);
            if (filename) {
                memcpy(filename, Data, Size);
                filename[Size] = '\0';
                curl_mime_filename(part, filename);
                free(filename);
            }
            /* Also test with NULL to trigger error path */
            curl_mime_filename(part, NULL);
        }
        curl_mime_free(mime);
    }

    /* 3. Test curl_easy_perform, curl_easy_upkeep, curl_easy_getinfo */
    CURL *curl = curl_easy_init();
    if (curl) {
        /* Set a simple URL using input data if it looks like a URL, otherwise use a safe default */
        /* We construct a safe URL to avoid network dependency issues in fuzzer, 
           but we still pass the data to test string handling */
        char *url = (char *)malloc(Size + 10);
        if (url) {
            /* Create a safe URL prefix to prevent invalid URL parsing crashes if data is garbage */
            snprintf(url, Size + 10, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            
            /* Set POSTFIELDS if data is available to test that path */
            /* Keep postdata alive until after curl_easy_perform returns to avoid use-after-free */
            char *postdata = NULL;
            if (Size > 0) {
                postdata = (char *)malloc(Size + 1);
                if (postdata) {
                    memcpy(postdata, Data, Size);
                    postdata[Size] = '\0';
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
                }
            }

            /* Perform the transfer (will likely fail or succeed locally depending on file) */
            curl_easy_perform(curl);

            /* Free postdata AFTER perform completes */
            if (postdata) {
                free(postdata);
            }

            /* Test curl_easy_upkeep */
            curl_easy_upkeep(curl);

            /* Test curl_easy_getinfo with various types */
            long response_code;
            double total_time;
            curl_off_t size_download;
            char *content_type;
            struct curl_slist *cookies;
            struct curl_certinfo *certinfo;

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);

            /* Cleanup getinfo results */
            if (cookies) curl_slist_free_all(cookies);
            /* Do NOT free certinfo manually; curl_easy_cleanup handles internal allocations */
            /* if (certinfo) curl_free(certinfo); <-- Removed to fix ASAN error */

            free(url);
        }

        /* Test with NULL handle for error paths */
        curl_easy_upkeep(NULL);
        curl_easy_perform(NULL);

        curl_easy_cleanup(curl);
    }

    /* Cleanup libcurl */
    curl_global_cleanup();

    return 0;
}