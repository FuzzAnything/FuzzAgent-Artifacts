// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
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

static void cleanup_easy(CURL *easy) {
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // curl_global_trace expects a null-terminated string.
    // The input Data is not guaranteed to be null-terminated.
    // We create a safe copy with explicit null termination.
    char config_buf[256];
    size_t config_len = (Size < 255) ? Size : 255;
    memcpy(config_buf, Data, config_len);
    config_buf[config_len] = '\0';
    
    curl_global_trace(config_buf);

    // Create a dummy file for curl_mime_filedata
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        remove("./dummy_file");
        return 0;
    }

    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        cleanup_easy(easy);
        remove("./dummy_file");
        return 0;
    }

    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        cleanup_mime(mime);
        cleanup_easy(easy);
        remove("./dummy_file");
        return 0;
    }

    // Fuzz curl_mime_encoder
    // Ensure null termination for encoding string
    char encoding_buf[256];
    size_t enc_len = (Size < 255) ? Size : 255;
    memcpy(encoding_buf, Data, enc_len);
    encoding_buf[enc_len] = '\0';
    curl_mime_encoder(part, encoding_buf);
    curl_mime_encoder(part, NULL);

    // Fuzz curl_mime_name
    char name_buf[256];
    size_t name_len = (Size < 255) ? Size : 255;
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';
    curl_mime_name(part, name_buf);
    curl_mime_name(part, NULL);

    // Fuzz curl_mime_filedata
    curl_mime_filedata(part, "./dummy_file");

    // Fuzz curl_mime_filename
    char filename_buf[256];
    size_t fname_len = (Size < 255) ? Size : 255;
    memcpy(filename_buf, Data, fname_len);
    filename_buf[fname_len] = '\0';
    curl_mime_filename(part, filename_buf);

    // Fuzz curl_easy_strerror with various error codes
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLcode code = (CURLcode)Data[i];
        const char *str = curl_easy_strerror(code);
        (void)str;
    }

    // Cleanup
    cleanup_mime(mime);
    cleanup_easy(easy);
    remove("./dummy_file");

    return 0;
}