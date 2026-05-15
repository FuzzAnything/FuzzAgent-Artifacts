// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

/* Callback functions for curl_mime_data_cb */
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    size_t total_size = size * nitems;
    /* Simulate reading data - in reality would read from arg */
    return total_size;
}

static int seek_callback(void *arg, curl_off_t offset, int origin) {
    /* Simulate seeking */
    return CURL_SEEKFUNC_OK;
}

static void free_callback(void *arg) {
    /* Simulate freeing resources */
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    /* Initialize mime structure */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    /* Add a mime part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    /* Use first byte to select which API to test */
    uint8_t selector = Size > 0 ? Data[0] % 6 : 0;
    const uint8_t *remaining_data = Size > 0 ? Data + 1 : Data;
    size_t remaining_size = Size > 0 ? Size - 1 : 0;

    switch (selector) {
        case 0: {
            /* Test curl_mime_filename */
            char filename[256];
            if (remaining_size > 0 && remaining_size < sizeof(filename)) {
                memcpy(filename, remaining_data, remaining_size);
                filename[remaining_size] = '\0';
                curl_mime_filename(part, filename);
            } else if (remaining_size >= sizeof(filename)) {
                memcpy(filename, remaining_data, sizeof(filename) - 1);
                filename[sizeof(filename) - 1] = '\0';
                curl_mime_filename(part, filename);
            }
            break;
        }
        case 1: {
            /* Test curl_mime_encoder */
            char encoding[256];
            if (remaining_size > 0 && remaining_size < sizeof(encoding)) {
                memcpy(encoding, remaining_data, remaining_size);
                encoding[remaining_size] = '\0';
                curl_mime_encoder(part, encoding);
            } else if (remaining_size >= sizeof(encoding)) {
                memcpy(encoding, remaining_data, sizeof(encoding) - 1);
                encoding[sizeof(encoding) - 1] = '\0';
                curl_mime_encoder(part, encoding);
            }
            break;
        }
        case 2: {
            /* Test curl_mime_data */
            if (remaining_size > 0) {
                curl_mime_data(part, (const char *)remaining_data, remaining_size);
            }
            break;
        }
        case 3: {
            /* Test curl_mime_data_cb */
            curl_off_t datasize = remaining_size;
            curl_mime_data_cb(part, datasize, read_callback, seek_callback, free_callback, NULL);
            break;
        }
        case 4: {
            /* Test curl_mime_filedata */
            /* Write data to dummy file */
            FILE *fp = fopen("./dummy_file", "wb");
            if (fp) {
                if (remaining_size > 0) {
                    fwrite(remaining_data, 1, remaining_size, fp);
                }
                fclose(fp);
                curl_mime_filedata(part, "./dummy_file");
                /* Clean up the dummy file */
                remove("./dummy_file");
            }
            break;
        }
        case 5: {
            /* Test curl_mime_addpart with additional part */
            curl_mimepart *second_part = curl_mime_addpart(mime);
            if (second_part && remaining_size > 0) {
                /* Use some data with the second part */
                curl_mime_data(second_part, (const char *)remaining_data, 
                              remaining_size > 100 ? 100 : remaining_size);
            }
            break;
        }
    }

    /* Clean up */
    curl_mime_free(mime);
    return 0;
}