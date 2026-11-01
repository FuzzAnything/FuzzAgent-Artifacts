// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
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

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment if needed, though most target functions
       do not strictly require it for basic error handling tests. */
    curl_global_init(CURL_GLOBAL_ALL);

    /* 1. Test curl_easy_strerror with various CURLcode values derived from input */
    CURLcode error_code = (CURLcode)(Data[0] % 100);
    const char *str = curl_easy_strerror(error_code);
    (void)str; /* Suppress unused variable warning */

    /* 2. Test curl_global_trace with input data as config string */
    if (Size > 1) {
        char *config_str = (char *)malloc(Size + 1);
        if (config_str) {
            memcpy(config_str, Data, Size);
            config_str[Size] = '\0';
            curl_global_trace(config_str);
            free(config_str);
        }
    }

    /* 3. Test MIME functions: curl_mime, curl_mimepart, curl_mime_type,
       curl_mime_encoder, curl_mime_filedata */
    
    /* Create a MIME structure */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        curl_global_cleanup();
        return 0;
    }

    /* Add a part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_global_cleanup();
        return 0;
    }

    /* Test curl_mime_type with input data as MIME type string */
    if (Size > 2) {
        char *mime_type = (char *)malloc(Size - 1);
        if (mime_type) {
            memcpy(mime_type, Data + 1, Size - 2);
            mime_type[Size - 2] = '\0';
            curl_mime_type(part, mime_type);
            free(mime_type);
        }
    }

    /* Test curl_mime_encoder with input data as encoding name */
    if (Size > 3) {
        char *encoding = (char *)malloc(Size - 2);
        if (encoding) {
            memcpy(encoding, Data + 2, Size - 3);
            encoding[Size - 3] = '\0';
            curl_mime_encoder(part, encoding);
            free(encoding);
        }
    }

    /* Test curl_mime_filedata with a dummy file */
    const char *dummy_file = "./dummy_file";
    write_dummy_file(dummy_file, Data, Size);
    curl_mime_filedata(part, dummy_file);

    /* Cleanup MIME structures */
    curl_mime_free(mime);

    /* Cleanup dummy file */
    remove(dummy_file);

    /* Cleanup curl global environment */
    curl_global_cleanup();

    return 0;
}