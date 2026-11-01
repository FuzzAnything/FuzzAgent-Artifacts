// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

/* Helper function to extract a null-terminated string from fuzz input safely */
static const char *get_string(const uint8_t *data, size_t size, size_t *consumed) {
    if (size == 0) {
        if (consumed) *consumed = 0;
        return NULL;
    }
    
    size_t len = 0;
    while (len < size && data[len] != '\0') {
        len++;
    }
    
    /* Ensure we don't return a string that goes beyond buffer bounds */
    if (len == size) {
        /* No null terminator found within bounds, treat as invalid or skip */
        if (consumed) *consumed = 0;
        return NULL;
    }
    
    if (consumed) *consumed = len + 1;
    return (const char *)data;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a MIME structure */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        curl_global_cleanup();
        return 0;
    }

    /* Add a part to the MIME structure */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_global_cleanup();
        return 0;
    }

    /* Extract data segments for fuzzing */
    size_t offset = 0;
    size_t remaining = Size;

    /* Fuzz curl_mime_filename */
    if (remaining >= 1) {
        size_t consumed;
        const char *filename = get_string(&Data[offset], remaining, &consumed);
        if (filename && consumed > 0) {
            curl_mime_filename(part, filename);
            offset += consumed;
            remaining -= consumed;
        }
    }

    /* Fuzz curl_mime_type */
    if (remaining >= 1) {
        size_t consumed;
        const char *mimetype = get_string(&Data[offset], remaining, &consumed);
        if (mimetype && consumed > 0) {
            curl_mime_type(part, mimetype);
            offset += consumed;
            remaining -= consumed;
        }
    }

    /* Fuzz curl_mime_data */
    if (remaining >= 1) {
        size_t consumed;
        const char *data = get_string(&Data[offset], remaining, &consumed);
        if (data && consumed > 0) {
            /* Use remaining data as payload, but limit size to prevent huge allocations */
            size_t datasize = (remaining - consumed) > 4096 ? 4096 : (remaining - consumed);
            curl_mime_data(part, data, datasize);
            offset += consumed + datasize;
            remaining -= consumed + datasize;
        }
    }

    /* Fuzz curl_mime_filedata */
    if (remaining >= 1) {
        size_t consumed;
        const char *filename = get_string(&Data[offset], remaining, &consumed);
        if (filename && consumed > 0) {
            /* Create a dummy file if needed */
            FILE *f = fopen("./dummy_file", "wb");
            if (f) {
                fwrite(Data, 1, Size, f);
                fclose(f);
            }
            curl_mime_filedata(part, "./dummy_file");
            offset += consumed;
            remaining -= consumed;
        }
    }

    /* Fuzz curl_mime_encoder */
    if (remaining >= 1) {
        size_t consumed;
        const char *encoding = get_string(&Data[offset], remaining, &consumed);
        if (encoding && consumed > 0) {
            curl_mime_encoder(part, encoding);
            offset += consumed;
            remaining -= consumed;
        }
    }

    /* Fuzz curl_mime_name */
    if (remaining >= 1) {
        size_t consumed;
        const char *name = get_string(&Data[offset], remaining, &consumed);
        if (name && consumed > 0) {
            curl_mime_name(part, name);
            offset += consumed;
            remaining -= consumed;
        }
    }

    /* Cleanup */
    curl_mime_free(mime);
    curl_global_cleanup();

    return 0;
}