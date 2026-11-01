// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
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

static char *extract_string(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) return NULL;
    size_t end = *offset;
    while (end < Size && Data[end] != 0) {
        end++;
    }
    if (end == *offset) {
        *offset = Size; // Skip empty strings
        return NULL;
    }
    size_t len = end - *offset;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, Data + *offset, len);
    str[len] = '\0';
    *offset = end + 1; // Skip the null terminator
    return str;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize a dummy file for file-based tests
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Create a new MIME structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        remove(dummy_file);
        return 0;
    }

    // Add a part to the MIME structure
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        cleanup_mime(mime);
        remove(dummy_file);
        return 0;
    }

    size_t offset = 0;

    // Fuzz curl_mime_type
    char *mimetype = extract_string(Data, Size, &offset);
    if (mimetype) {
        curl_mime_type(part, mimetype);
        free(mimetype);
    } else {
        curl_mime_type(part, NULL);
    }

    // Fuzz curl_mime_filename
    char *filename = extract_string(Data, Size, &offset);
    if (filename) {
        curl_mime_filename(part, filename);
        free(filename);
    } else {
        curl_mime_filename(part, NULL);
    }

    // Fuzz curl_mime_encoder
    char *encoding = extract_string(Data, Size, &offset);
    if (encoding) {
        curl_mime_encoder(part, encoding);
        free(encoding);
    } else {
        curl_mime_encoder(part, NULL);
    }

    // Fuzz curl_mime_data
    if (offset < Size) {
        size_t datasize = Size - offset;
        // Ensure datasize is not too large to avoid allocation issues
        if (datasize > 1024 * 1024) {
            datasize = 1024 * 1024;
        }
        char *data = (char *)malloc(datasize);
        if (data) {
            memcpy(data, Data + offset, datasize);
            curl_mime_data(part, data, datasize);
            free(data);
        }
    } else {
        // Fuzz with NULL data
        curl_mime_data(part, NULL, 0);
    }

    // Fuzz curl_mime_filedata
    if (fp) {
        curl_mime_filedata(part, dummy_file);
    } else {
        curl_mime_filedata(part, NULL);
    }

    // Fuzz curl_mime_name
    char *name = extract_string(Data, Size, &offset);
    if (name) {
        curl_mime_name(part, name);
        free(name);
    } else {
        curl_mime_name(part, NULL);
    }

    // Cleanup
    cleanup_mime(mime);
    remove(dummy_file);

    return 0;
}