// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        const char *dummy = "dummy content for fuzzing";
        fwrite(dummy, 1, strlen(dummy), f);
        fclose(f);
    }
}

/* Dummy read callback for curl_mime_data_cb */
static size_t dummy_read_cb(char *buffer, size_t size, size_t nitems, void *userp) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userp;
    return 0;
}

/* Dummy seek callback for curl_mime_data_cb */
static int dummy_seek_cb(void *userp, curl_off_t offset, int origin) {
    (void)userp;
    (void)offset;
    (void)origin;
    return 0;
}

/* Dummy free callback for curl_mime_data_cb */
static void dummy_free_cb(void *userp) {
    (void)userp;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl globally once per process is ideal, but for fuzzing
     * we do it here to ensure state is reset if the fuzzer restarts the process.
     * In a real app, this is called once at startup. */
    CURLcode init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (init_res != CURLE_OK) {
        return 0;
    }

    /* Create a dummy file for potential file-based operations */
    create_dummy_file("./dummy_file");

    /* Initialize a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Initialize a MIME structure */
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Add a part to the MIME structure */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to drive various API calls */
    size_t idx = 0;
    while (idx < Size) {
        uint8_t op = Data[idx++];
        
        switch (op % 5) {
            case 0:
                /* Test curl_easy_setopt with URL (string option) */
                if (idx + 1 < Size) {
                    size_t len = Data[idx];
                    idx++;
                    if (len > 0 && idx + len <= Size) {
                        char *url = (char *)malloc(len + 1);
                        if (url) {
                            memcpy(url, &Data[idx], len);
                            url[len] = '\0';
                            curl_easy_setopt(curl, CURLOPT_URL, url);
                            free(url);
                        }
                        idx += len;
                    }
                }
                break;

            case 1:
                /* Test curl_easy_setopt with long option (e.g., CURLOPT_TIMEOUT) */
                if (idx < Size) {
                    long timeout = (long)Data[idx];
                    idx++;
                    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
                }
                break;

            case 2:
                /* Test curl_easy_upkeep */
                curl_easy_upkeep(curl);
                break;

            case 3:
                /* Test curl_mime_data_cb with callbacks */
                if (idx < Size) {
                    curl_off_t datasize = (curl_off_t)Data[idx];
                    idx++;
                    curl_mime_data_cb(part, datasize, dummy_read_cb, dummy_seek_cb, dummy_free_cb, (void *)Data);
                }
                break;

            case 4:
                /* Test curl_easy_perform */
                /* Ensure URL is set before performing */
                if (idx < Size) {
                    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
                    curl_easy_perform(curl);
                }
                break;
        }
    }

    /* Cleanup */
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}