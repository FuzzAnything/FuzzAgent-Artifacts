// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
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

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *content = "This is a dummy file for fuzzing curl MIME functions.\n";
        fwrite(content, 1, strlen(content), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize global tracing if data suggests it (first byte > 0)
    if (Data[0] != 0) {
        // Ensure null termination for the config string
        char *config = malloc(Size + 1);
        if (config) {
            memcpy(config, Data, Size);
            config[Size] = '\0';
            curl_global_trace(config);
            free(config);
        }
    }

    // Setup dummy file for file-based operations
    setup_dummy_file();

    // Create a MIME multipart structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    // Add a part to the MIME structure
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    // Fuzz curl_mime_filename
    // Use a portion of the input data as filename
    if (Size > 1) {
        char *filename = malloc(Size);
        if (filename) {
            memcpy(filename, Data, Size);
            filename[Size - 1] = '\0'; // Ensure null termination
            curl_mime_filename(part, filename);
            free(filename);
        }
    }

    // Fuzz curl_mime_encoder
    // Use a portion of the input data as encoding name
    if (Size > 2) {
        char *encoding = malloc(Size);
        if (encoding) {
            memcpy(encoding, Data + 1, Size - 1);
            encoding[Size - 1] = '\0';
            curl_mime_encoder(part, encoding);
            free(encoding);
        }
    }

    // Fuzz curl_mime_type
    // Use a portion of the input data as mimetype
    if (Size > 3) {
        char *mimetype = malloc(Size);
        if (mimetype) {
            memcpy(mimetype, Data + 2, Size - 2);
            mimetype[Size - 2] = '\0';
            curl_mime_type(part, mimetype);
            free(mimetype);
        }
    }

    // Fuzz curl_mime_filedata
    // Use the dummy file
    curl_mime_filedata(part, "./dummy_file");

    // Fuzz curl_easy_strerror with various error codes derived from input
    // Map input bytes to CURLcode values (0-100 range for safety)
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLcode code = (CURLcode)(Data[i] % 100);
        const char *error_str = curl_easy_strerror(code);
        (void)error_str; // Suppress unused variable warning
    }

    // Cleanup
    curl_mime_free(mime);

    return 0;
}