// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

static void cleanup_mime(curl_mime *mime) {
    if (mime) {
        curl_mime_free(mime);
    }
}

static char *get_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    size_t len = 0;
    while (*offset + len < size && data[*offset + len] != '\0') {
        len++;
    }
    if (len == 0) {
        (*offset)++;
        return NULL;
    }
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len + 1;
    return str;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        cleanup_mime(mime);
        return 0;
    }

    size_t offset = 0;
    
    // Fuzz curl_mime_encoder
    {
        char *encoding = get_string(Data, Size, &offset);
        if (encoding) {
            curl_mime_encoder(part, encoding);
            free(encoding);
        } else {
            curl_mime_encoder(part, NULL);
        }
    }

    // Fuzz curl_mime_filename
    {
        char *filename = get_string(Data, Size, &offset);
        if (filename) {
            curl_mime_filename(part, filename);
            free(filename);
        } else {
            curl_mime_filename(part, NULL);
        }
    }

    // Fuzz curl_mime_filedata
    {
        // Write dummy file content
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }
        curl_mime_filedata(part, "./dummy_file");
    }

    // Fuzz curl_mime_data
    {
        if (offset < Size) {
            size_t datasize = (Size - offset) > 1024 ? 1024 : (Size - offset);
            if (datasize > 0) {
                curl_mime_data(part, (const char *)(Data + offset), datasize);
                offset += datasize;
            }
        }
        // Also test with CURL_ZERO_TERMINATED if we can form a valid string
        char *str_data = get_string(Data, Size, &offset);
        if (str_data) {
            curl_mime_data(part, str_data, CURL_ZERO_TERMINATED);
            free(str_data);
        }
    }

    // Fuzz curl_mime_name
    {
        char *name = get_string(Data, Size, &offset);
        if (name) {
            curl_mime_name(part, name);
            free(name);
        } else {
            curl_mime_name(part, NULL);
        }
    }

    // Fuzz curl_mime_type
    {
        char *mimetype = get_string(Data, Size, &offset);
        if (mimetype) {
            curl_mime_type(part, mimetype);
            free(mimetype);
        } else {
            curl_mime_type(part, NULL);
        }
    }

    // Cleanup
    cleanup_mime(mime);
    return 0;
}