// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for curl_mime_filedata testing
    write_dummy_file("./dummy_file", Data, Size);

    // Initialize curl_mime structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    // Fuzz curl_mime_filename
    // Use input data as filename, ensuring it's null-terminated
    char *filename = (char *)malloc(Size + 1);
    if (filename) {
        memcpy(filename, Data, Size);
        filename[Size] = '\0';
        curl_mime_filename(part, filename);
        free(filename);
    }

    // Fuzz curl_mime_name
    char *name = (char *)malloc(Size + 1);
    if (name) {
        memcpy(name, Data, Size);
        name[Size] = '\0';
        curl_mime_name(part, name);
        free(name);
    }

    // Fuzz curl_mime_filedata
    curl_mime_filedata(part, "./dummy_file");

    // Fuzz curl_slist_append and curl_mime_headers
    struct curl_slist *headers = NULL;
    // Split data into chunks to create multiple headers
    size_t chunk_size = Size / 4;
    if (chunk_size == 0) chunk_size = Size;
    
    for (size_t i = 0; i < 4 && (i * chunk_size < Size); i++) {
        size_t current_chunk = (i + 1) * chunk_size > Size ? Size - i * chunk_size : chunk_size;
        char *header_data = (char *)malloc(current_chunk + 2);
        if (header_data) {
            memcpy(header_data, Data + i * chunk_size, current_chunk);
            header_data[current_chunk] = ':';
            header_data[current_chunk + 1] = '\0';
            headers = curl_slist_append(headers, header_data);
            free(header_data);
        }
    }

    if (headers) {
        // Test with take_ownership = 1
        curl_mime_headers(part, headers, 1);
        // headers is now owned by libcurl, do not free manually
    } else {
        // Test with NULL headers
        curl_mime_headers(part, NULL, 0);
    }

    // Additional fuzzing: try appending more headers without ownership
    struct curl_slist *more_headers = NULL;
    for (size_t i = 0; i < 3; i++) {
        char *h = (char *)malloc(Size / 2 + 1);
        if (h) {
            memcpy(h, Data, Size / 2);
            h[Size / 2] = '\0';
            more_headers = curl_slist_append(more_headers, h);
            free(h);
        }
    }
    if (more_headers) {
        curl_mime_headers(part, more_headers, 0);
        curl_slist_free_all(more_headers);
    }

    // Cleanup
    curl_mime_free(mime);
    return 0;
}