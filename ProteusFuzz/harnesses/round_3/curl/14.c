// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment if needed, though not strictly required for these specific functions */
    /* curl_global_init(CURL_GLOBAL_DEFAULT); */

    /* Create a dummy file for file-based operations */
    const char *dummy_file = "./dummy_file";
    create_dummy_file(dummy_file, Data, Size);

    /* Create a mime structure and a part */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    /* 1. Fuzz curl_mime_type */
    /* Ensure null termination for string arguments */
    char *type_str = (char *)malloc(Size + 1);
    if (type_str) {
        memcpy(type_str, Data, Size);
        type_str[Size] = '\0';
        curl_mime_type(part, type_str);
        /* Also test with NULL to clear type */
        curl_mime_type(part, NULL);
        free(type_str);
    }

    /* 2. Fuzz curl_global_trace */
    /* Ensure null termination for config string */
    char *config_str = (char *)malloc(Size + 1);
    if (config_str) {
        memcpy(config_str, Data, Size);
        config_str[Size] = '\0';
        curl_global_trace(config_str);
        free(config_str);
    }

    /* 3. Fuzz curl_mime_filename */
    char *filename_str = (char *)malloc(Size + 1);
    if (filename_str) {
        memcpy(filename_str, Data, Size);
        filename_str[Size] = '\0';
        curl_mime_filename(part, filename_str);
        free(filename_str);
    }

    /* 4. Fuzz curl_easy_strerror */
    /* Use various CURLcode values */
    (void)curl_easy_strerror(CURLE_OK);
    (void)curl_easy_strerror(CURLE_COULDNT_CONNECT);
    (void)curl_easy_strerror(CURLE_BAD_FUNCTION_ARGUMENT);
    (void)curl_easy_strerror(CURLE_OUT_OF_MEMORY);
    (void)curl_easy_strerror(CURLE_READ_ERROR);
    /* Also fuzz with raw data interpreted as CURLcode (cast to int) */
    if (Size >= sizeof(CURLcode)) {
        CURLcode fuzzed_code = *(const CURLcode *)(Data);
        (void)curl_easy_strerror(fuzzed_code);
    }

    /* 5. Fuzz curl_mime_data */
    if (Size > 0) {
        curl_mime_data(part, (const char *)Data, Size);
        /* Test with CURL_ZERO_TERMINATED if data contains null */
        size_t null_pos = strnlen((const char *)Data, Size);
        if (null_pos < Size) {
            curl_mime_data(part, (const char *)Data, CURL_ZERO_TERMINATED);
        }
    }

    /* 6. Fuzz curl_mime_filedata */
    /* Use the dummy file path */
    curl_mime_filedata(part, dummy_file);

    /* Cleanup */
    curl_mime_free(mime);
    remove(dummy_file);

    /* Cleanup global init if performed */
    /* curl_global_cleanup(); */

    return 0;
}