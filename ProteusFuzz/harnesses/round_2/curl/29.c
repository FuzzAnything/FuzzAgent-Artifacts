// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type checks if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl globally */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* 1. Fuzz curl_global_trace */
    /* Ensure null termination for the config string */
    char *config_str = malloc(Size + 1);
    if (config_str) {
        memcpy(config_str, Data, Size);
        config_str[Size] = '\0';
        curl_global_trace(config_str);
        free(config_str);
    }

    /* 2. Fuzz curl_easy_upkeep */
    CURL *curl = curl_easy_init();
    if (curl) {
        /* Basic upkeep test */
        curl_easy_upkeep(curl);

        /* Set some options to explore different states before upkeep */
        /* Use a dummy URL to ensure handle is somewhat configured */
        if (Size > 10) {
            char url_buf[256];
            size_t url_len = (Size < 255) ? Size : 255;
            memcpy(url_buf, Data, url_len);
            url_buf[url_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url_buf);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
            curl_easy_upkeep(curl);
        }

        /* 4. Fuzz curl_easy_perform */
        /* Ensure we have a valid URL set before performing */
        if (Size > 5) {
            char url_buf[256];
            size_t url_len = (Size < 255) ? Size : 255;
            memcpy(url_buf, Data, url_len);
            url_buf[url_len] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url_buf);
            /* Set a small buffer for error handling to avoid crashes on bad input */
            char errbuf[CURL_ERROR_SIZE];
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_perform(curl);
        }

        /* 5. Fuzz curl_mime_filedata and 6. curl_mime_subparts */
        /* Create a dummy file for testing file-based MIME parts */
        FILE *dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            if (Size > 0) {
                fwrite(Data, 1, Size, dummy_file);
            }
            fclose(dummy_file);

            curl_mime *mime = curl_mime_init(curl);
            if (mime) {
                curl_mimepart *part = curl_mime_addpart(mime);
                if (part) {
                    /* Test curl_mime_filedata with the dummy file */
                    curl_mime_filedata(part, "./dummy_file");

                    /* Test curl_mime_subparts */
                    curl_mime *subparts = curl_mime_init(curl);
                    if (subparts) {
                        curl_mimepart *subpart = curl_mime_addpart(subparts);
                        if (subpart) {
                            /* Set some data for the subpart */
                            curl_mime_data(subpart, (char *)Data, Size);
                            curl_mime_subparts(part, subparts);
                        }
                        curl_mime_free(subparts);
                    }
                }
                curl_mime_free(mime);
            }
        }

        /* Cleanup curl handle */
        curl_easy_cleanup(curl);
    }

    /* Cleanup global init */
    curl_global_cleanup();

    return 0;
}