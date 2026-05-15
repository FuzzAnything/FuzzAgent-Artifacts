// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "curl/curl.h"

static size_t read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    size_t total_size = size * nitems;
    const uint8_t *data = (const uint8_t *)arg;
    static size_t offset = 0;
    
    if (offset < total_size) {
        size_t to_copy = total_size - offset;
        if (to_copy > size * nitems) to_copy = size * nitems;
        memcpy(buffer, data + offset, to_copy);
        offset += to_copy;
        return to_copy;
    }
    return 0;
}

static int seek_callback(void *arg, curl_off_t offset, int origin) {
    return CURL_SEEKFUNC_OK;
}

static void free_callback(void *arg) {
    // Nothing to free since we use static data
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create dummy file for filedata testing
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Initialize mime structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        if (access("./dummy_file", F_OK) == 0) {
            unlink("./dummy_file");
        }
        return 0;
    }
    
    // Add a part to work with
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        if (access("./dummy_file", F_OK) == 0) {
            unlink("./dummy_file");
        }
        return 0;
    }
    
    // Test curl_mime_name with various inputs
    if (Size > 0) {
        // Create null-terminated string for name
        char *name = malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            curl_mime_name(part, name);
            free(name);
        }
        
        // Test with NULL name
        curl_mime_name(part, NULL);
    }
    
    // Test curl_mime_filedata
    curl_mime_filedata(part, "./dummy_file");
    // Test with NULL filename
    curl_mime_filedata(part, NULL);
    
    // Test curl_mime_data
    if (Size > 0) {
        char *data_copy = malloc(Size);
        if (data_copy) {
            memcpy(data_copy, Data, Size);
            curl_mime_data(part, data_copy, Size);
            free(data_copy);
        }
    }
    
    // Test curl_mime_headers
    if (Size > 0) {
        struct curl_slist *headers = NULL;
        char *header_str = malloc(Size + 12);
        if (header_str) {
            memcpy(header_str, "X-Test: ", 8);
            memcpy(header_str + 8, Data, Size > 100 ? 100 : Size);
            header_str[8 + (Size > 100 ? 100 : Size)] = '\0';
            headers = curl_slist_append(headers, header_str);
            free(header_str);
        }
        
        // Test with take_ownership = 0
        curl_mime_headers(part, headers, 0);
        curl_slist_free_all(headers);
        
        // Test with take_ownership = 1
        headers = curl_slist_append(NULL, "Content-Type: text/plain");
        curl_mime_headers(part, headers, 1);
        
        // Test with NULL headers
        curl_mime_headers(part, NULL, 0);
    }
    
    // Test curl_mime_encoder
    curl_mime_encoder(part, "base64");
    curl_mime_encoder(part, "quoted-printable");
    curl_mime_encoder(part, "8bit");
    curl_mime_encoder(part, "binary");
    curl_mime_encoder(part, NULL);
    
    // Test curl_mime_data_cb
    if (Size > 0) {
        curl_mime_data_cb(part, (curl_off_t)Size, 
                         read_callback, 
                         seek_callback, 
                         free_callback, 
                         (void *)Data);
    }
    
    // Clean up
    curl_mime_free(mime);
    if (access("./dummy_file", F_OK) == 0) {
        unlink("./dummy_file");
    }
    
    return 0;
}