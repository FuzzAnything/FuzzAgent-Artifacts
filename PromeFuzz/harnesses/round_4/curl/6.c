// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy callbacks for curl_mime_data_cb */
static size_t dummy_readfunc(char *buffer, size_t size, size_t nitems, void *arg) {
    return 0;
}

static int dummy_seekfunc(void *arg, curl_off_t offset, int origin) {
    return CURL_SEEKFUNC_OK;
}

static void dummy_freefunc(void *arg) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    /* Initialize dummy file if needed */
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy);
        }
        fclose(dummy);
    }

    /* Create CURL handle and mime structure */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Fuzz curl_mime_name */
    if (Size > 0) {
        char *name = malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            curl_mime_name(part, name);
            free(name);
        }
        /* Also test with NULL */
        curl_mime_name(part, NULL);
    }

    /* Fuzz curl_mime_filedata */
    curl_mime_filedata(part, "./dummy_file");
    /* Test with NULL */
    curl_mime_filedata(part, NULL);

    /* Fuzz curl_mime_headers */
    struct curl_slist *headers = NULL;
    if (Size > 0) {
        char *header = malloc(Size + 1);
        if (header) {
            memcpy(header, Data, Size);
            header[Size] = '\0';
            headers = curl_slist_append(headers, header);
            free(header);
        }
    }
    /* Test with ownership */
    curl_mime_headers(part, headers, 1);
    /* Test without ownership */
    headers = curl_slist_append(NULL, "Test: Header");
    curl_mime_headers(part, headers, 0);
    curl_slist_free_all(headers);
    /* Test with NULL */
    curl_mime_headers(part, NULL, 0);

    /* Fuzz curl_mime_data */
    if (Size > 0) {
        curl_mime_data(part, (const char *)Data, Size);
    }
    /* Test with zero size */
    curl_mime_data(part, "", 0);

    /* Fuzz curl_mime_encoder */
    if (Size > 0) {
        char *encoding = malloc(Size + 1);
        if (encoding) {
            memcpy(encoding, Data, Size);
            encoding[Size] = '\0';
            curl_mime_encoder(part, encoding);
            free(encoding);
        }
    }
    /* Test with NULL */
    curl_mime_encoder(part, NULL);

    /* Fuzz curl_mime_data_cb */
    curl_mime_data_cb(part, (curl_off_t)Size,
                      dummy_readfunc,
                      dummy_seekfunc,
                      dummy_freefunc,
                      NULL);

    /* Cleanup */
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    remove("./dummy_file");

    return 0;
}