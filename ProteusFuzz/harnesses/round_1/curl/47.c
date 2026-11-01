// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
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

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl if not already done (thread-safe) */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a MIME structure */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a MIME part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_global_cleanup();
        return 0;
    }

    /* Ensure Data is treated as a C string for API calls */
    /* We use the input data directly, ensuring null termination where needed */
    size_t len = Size;
    char *safe_data = NULL;
    
    if (len > 0 && Data[len - 1] != '\0') {
        /* Create a temporary buffer with null termination if needed */
        safe_data = malloc(len + 1);
        if (safe_data) {
            memcpy(safe_data, Data, len);
            safe_data[len] = '\0';
            len = strlen(safe_data);
        } else {
            curl_mime_free(mime);
            curl_global_cleanup();
            return 0;
        }
    }

    const char *input_str = safe_data ? safe_data : (const char *)Data;

    /* Test curl_mime_type */
    if (len > 0) {
        curl_mime_type(part, input_str);
    } else {
        curl_mime_type(part, NULL);
    }

    /* Test curl_mime_name */
    if (len > 0) {
        curl_mime_name(part, input_str);
    } else {
        curl_mime_name(part, NULL);
    }

    /* Test curl_mime_filename */
    if (len > 0) {
        curl_mime_filename(part, input_str);
    } else {
        curl_mime_filename(part, NULL);
    }

    /* Test curl_mime_encoder */
    /* Try with a valid encoding name or NULL */
    if (len >= 6 && memcmp(input_str, "base64", 6) == 0) {
        curl_mime_encoder(part, "base64");
    } else if (len >= 16 && memcmp(input_str, "quoted-printable", 16) == 0) {
        curl_mime_encoder(part, "quoted-printable");
    } else {
        curl_mime_encoder(part, NULL);
    }

    /* Test curl_mime_filedata */
    /* Create a dummy file to test file data source */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        /* Write some data to the dummy file */
        fwrite(Data, 1, Size, fp);
        fclose(fp);

        /* Attempt to set file data */
        curl_mime_filedata(part, dummy_file);
    }

    /* Cleanup */
    curl_mime_free(mime);
    curl_global_cleanup();

    /* Remove dummy file */
    remove(dummy_file);

    /* Free allocated buffer if used */
    if (safe_data) {
        free(safe_data);
    }

    return 0;
}