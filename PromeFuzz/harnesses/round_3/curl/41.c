// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

/* Structure to hold callback data */
struct cb_data {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

/* Callback functions for curl_mime_data_cb */
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    size_t total_size = size * nitems;
    struct cb_data *data = (struct cb_data *)arg;
    size_t to_copy = data->size < total_size ? data->size : total_size;
    
    if (to_copy > 0 && data->offset < data->size) {
        memcpy(buffer, data->data + data->offset, to_copy);
        data->offset += to_copy;
        return to_copy;
    }
    return 0; /* EOF */
}

static int seek_callback(void *arg, curl_off_t offset, int origin) {
    struct cb_data *data = (struct cb_data *)arg;
    
    switch (origin) {
        case SEEK_SET:
            if (offset >= 0 && offset <= (curl_off_t)data->size) {
                data->offset = offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_CUR:
            if (data->offset + offset >= 0 && data->offset + offset <= (curl_off_t)data->size) {
                data->offset += offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_END:
            if (data->size + offset >= 0 && data->size + offset <= (curl_off_t)data->size) {
                data->offset = data->size + offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
    }
    return CURL_SEEKFUNC_FAIL;
}

static void free_callback(void *arg) {
    /* Nothing to free as we use fuzzer data */
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    /* Create a dummy file for filedata operations */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    /* Create curl easy handle and mime structure */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
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
    
    /* Use fuzzer data to decide which functions to call */
    if (Size > 0) {
        uint8_t selector = Data[0];
        const uint8_t *payload = Size > 1 ? Data + 1 : Data;
        size_t payload_size = Size > 1 ? Size - 1 : 1;
        
        switch (selector % 6) {
            case 0:
                curl_mime_data(part, (const char *)payload, payload_size);
                break;
            case 1:
                curl_mime_filedata(part, filename);
                break;
            case 2:
                if (payload_size > 0) {
                    char encoding[32];
                    size_t enc_len = payload_size < 31 ? payload_size : 31;
                    memcpy(encoding, payload, enc_len);
                    encoding[enc_len] = '\0';
                    curl_mime_encoder(part, encoding);
                } else {
                    curl_mime_encoder(part, "base64");
                }
                break;
            case 3:
                if (payload_size > 0) {
                    char fname[32];
                    size_t fname_len = payload_size < 31 ? payload_size : 31;
                    memcpy(fname, payload, fname_len);
                    fname[fname_len] = '\0';
                    curl_mime_filename(part, fname);
                }
                break;
            case 4:
                /* Already added part above, but we can add another */
                if (Size > 10) {
                    curl_mimepart *part2 = curl_mime_addpart(mime);
                    if (part2) {
                        curl_mime_data(part2, (const char *)payload, payload_size);
                    }
                }
                break;
            case 5:
                if (payload_size > 0) {
                    struct cb_data cb_data = {
                        .data = payload,
                        .size = payload_size,
                        .offset = 0
                    };
                    curl_mime_data_cb(part, (curl_off_t)payload_size,
                                     read_callback,
                                     seek_callback,
                                     free_callback,
                                     &cb_data);
                }
                break;
        }
        
        /* Try some combinations */
        if (selector % 3 == 0) {
            curl_mime_filename(part, NULL);
        }
        if (selector % 4 == 0) {
            curl_mime_encoder(part, NULL);
        }
    }
    
    /* Cleanup */
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    
    /* Remove dummy file */
    remove(filename);
    
    return 0;
}