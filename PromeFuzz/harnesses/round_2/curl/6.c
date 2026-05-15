// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Callback functions for curl_mime_data_cb */
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    size_t *offset = (size_t *)arg;
    const uint8_t *data = ((uint8_t **)arg)[0];
    size_t total_size = ((size_t *)arg)[1];
    size_t buffer_size = size * nitems;
    size_t remaining = total_size - *offset;
    size_t to_copy = buffer_size < remaining ? buffer_size : remaining;
    
    if (to_copy > 0) {
        memcpy(buffer, data + *offset, to_copy);
        *offset += to_copy;
    }
    return to_copy;
}

static int seek_callback(void *arg, curl_off_t offset, int origin) {
    size_t *current_offset = (size_t *)arg;
    const uint8_t *data = ((uint8_t **)arg)[0];
    size_t total_size = ((size_t *)arg)[1];
    
    switch (origin) {
        case SEEK_SET:
            if (offset >= 0 && offset <= (curl_off_t)total_size) {
                *current_offset = (size_t)offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_CUR:
            if (*current_offset + offset >= 0 && 
                *current_offset + offset <= total_size) {
                *current_offset += (size_t)offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_END:
            if (total_size + offset >= 0 && 
                total_size + offset <= total_size) {
                *current_offset = total_size + (size_t)offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
    }
    return CURL_SEEKFUNC_FAIL;
}

static void free_callback(void *arg) {
    /* Nothing to free as we use stack-allocated structure */
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    struct curl_slist *headers = NULL;
    FILE *dummy_file = NULL;
    size_t offset = 0;
    
    /* Initialize curl */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    /* Create mime structure */
    mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    /* Create mime part */
    part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    /* Test curl_mime_filedata */
    if (Size > 0) {
        dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy_file);
            fclose(dummy_file);
            
            curl_mime_filedata(part, "./dummy_file");
            curl_mime_filename(part, NULL);  /* Clear unwanted filename */
            
            /* Remove the dummy file */
            remove("./dummy_file");
        }
    }
    
    /* Test curl_mime_headers */
    if (Size > 10) {
        char header1[256], header2[256];
        size_t copy_size = Size > 255 ? 255 : Size;
        
        /* Create first header */
        memcpy(header1, Data, copy_size);
        header1[copy_size] = '\0';
        headers = curl_slist_append(headers, header1);
        
        /* Create second header if enough data */
        if (Size > copy_size + 10) {
            size_t second_size = Size - copy_size > 255 ? 255 : Size - copy_size;
            memcpy(header2, Data + copy_size, second_size);
            header2[second_size] = '\0';
            headers = curl_slist_append(headers, header2);
        }
        
        curl_mime_headers(part, headers, 1);  /* Give ownership to libcurl */
        headers = NULL;  /* List is now owned by libcurl */
    } else {
        /* Test with NULL headers to clear */
        curl_mime_headers(part, NULL, 0);
    }
    
    /* Test curl_mime_data */
    if (Size > 0) {
        /* Use first half of data for curl_mime_data */
        size_t data_size = Size / 2;
        if (data_size > 0) {
            char *data_copy = malloc(data_size);
            if (data_copy) {
                memcpy(data_copy, Data, data_size);
                curl_mime_data(part, data_copy, data_size);
                /* Note: curl_mime_data makes its own copy, so we can free */
                free(data_copy);
            }
        }
    }
    
    /* Test curl_mime_data_cb */
    if (Size > 0) {
        /* Prepare callback arguments */
        size_t cb_args[3];
        cb_args[0] = (size_t)Data;
        cb_args[1] = Size;
        cb_args[2] = 0;  /* offset */
        
        curl_mime_data_cb(part, (curl_off_t)Size,
                         read_callback,
                         seek_callback,
                         free_callback,
                         cb_args);
    }
    
    /* Test curl_mime_name */
    if (Size > 0) {
        char name[256];
        size_t name_size = Size > 255 ? 255 : Size;
        memcpy(name, Data, name_size);
        name[name_size] = '\0';
        curl_mime_name(part, name);
        
        /* Test with NULL to reset */
        if (Size % 2 == 0) {
            curl_mime_name(part, NULL);
        }
    }
    
    /* Test curl_mime_encoder */
    const char *encodings[] = {"base64", "binary", "8bit", "7bit", "quoted-printable", NULL};
    if (Size > 0) {
        int idx = Data[0] % 5;
        curl_mime_encoder(part, encodings[idx]);
        
        /* Test removing encoder */
        if (Size % 3 == 0) {
            curl_mime_encoder(part, NULL);
        }
    }
    
    /* Cleanup */
    if (headers) {
        curl_slist_free_all(headers);
    }
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}