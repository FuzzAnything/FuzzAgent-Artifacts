// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_data at mime.c:1273:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Callback functions for curl_mime_data_cb */
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    size_t total_size = size * nitems;
    size_t *offset = (size_t *)arg;
    static const char dummy_data[] = "dummy data for callback";
    size_t data_len = sizeof(dummy_data) - 1;
    
    if (*offset >= data_len) {
        return 0;
    }
    
    size_t to_copy = (total_size < data_len - *offset) ? total_size : data_len - *offset;
    memcpy(buffer, dummy_data + *offset, to_copy);
    *offset += to_copy;
    return to_copy;
}

static int seek_callback(void *arg, curl_off_t offset, int origin) {
    (void)arg;
    (void)offset;
    (void)origin;
    return CURL_SEEKFUNC_OK;
}

static void free_callback(void *arg) {
    free(arg);
}

/* Helper function to create dummy headers */
static struct curl_slist *create_dummy_headers(const uint8_t *data, size_t size) {
    struct curl_slist *headers = NULL;
    
    if (size > 0) {
        char *header1 = malloc(50);
        if (header1) {
            snprintf(header1, 50, "X-Custom-Header: %.*s", (int)(size > 20 ? 20 : size), data);
            headers = curl_slist_append(headers, header1);
            free(header1);
        }
        
        char *header2 = malloc(50);
        if (header2) {
            snprintf(header2, 50, "Content-Type: %.*s", (int)(size > 10 ? 10 : size), data + (size > 5 ? 5 : 0));
            headers = curl_slist_append(headers, header2);
            free(header2);
        }
    }
    
    return headers;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    /* Create a mime structure */
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        return 0;
    }
    
    /* Add a mime part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        return 0;
    }
    
    /* Use the first byte to decide which functions to call */
    uint8_t selector = Size > 0 ? Data[0] : 0;
    const uint8_t *fuzz_data = Size > 0 ? Data + 1 : Data;
    size_t fuzz_size = Size > 0 ? Size - 1 : 0;
    
    /* Test curl_mime_data */
    if (fuzz_size > 0) {
        char *data_copy = malloc(fuzz_size);
        if (data_copy) {
            memcpy(data_copy, fuzz_data, fuzz_size);
            curl_mime_data(part, data_copy, fuzz_size);
            free(data_copy);
        }
    }
    
    /* Test curl_mime_data_cb */
    if (selector & 0x01) {
        size_t *cb_arg = malloc(sizeof(size_t));
        if (cb_arg) {
            *cb_arg = 0;
            curl_mime_data_cb(part, (curl_off_t)fuzz_size, read_callback, seek_callback, free_callback, cb_arg);
        }
    }
    
    /* Test curl_mime_filedata */
    if (fuzz_size > 0) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(fuzz_data, 1, fuzz_size, fp);
            fclose(fp);
            
            curl_mime_filedata(part, "./dummy_file");
            
            /* Clear the filename if set */
            curl_mime_filename(part, NULL);
            
            /* Remove the dummy file */
            remove("./dummy_file");
        }
    }
    
    /* Test curl_mime_headers */
    if (selector & 0x02) {
        struct curl_slist *headers = create_dummy_headers(fuzz_data, fuzz_size);
        if (headers) {
            /* Test with take_ownership = 0 */
            curl_mime_headers(part, headers, 0);
            curl_slist_free_all(headers);
            
            /* Test with take_ownership = 1 */
            headers = create_dummy_headers(fuzz_data, fuzz_size);
            if (headers) {
                curl_mime_headers(part, headers, 1);
            }
        }
    }
    
    /* Test curl_mime_name */
    if (fuzz_size > 0) {
        char *name = malloc(fuzz_size + 1);
        if (name) {
            memcpy(name, fuzz_data, fuzz_size);
            name[fuzz_size] = '\0';
            curl_mime_name(part, name);
            free(name);
        }
    }
    
    /* Test curl_mime_encoder */
    if (selector & 0x04) {
        if (fuzz_size > 0) {
            char *encoding = malloc(fuzz_size + 1);
            if (encoding) {
                memcpy(encoding, fuzz_data, fuzz_size);
                encoding[fuzz_size] = '\0';
                curl_mime_encoder(part, encoding);
                free(encoding);
            }
        } else {
            curl_mime_encoder(part, "base64");
        }
    }
    
    /* Clean up */
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    
    return 0;
}