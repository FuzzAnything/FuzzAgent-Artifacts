// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare dummy file for curl_mime_filedata */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Initialize curl_mime and curl_mimepart */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    /* Ensure Data is treated as a null-terminated string for API calls */
    char *safe_string = NULL;
    if (Size > 0) {
        safe_string = (char *)malloc(Size + 1);
        if (safe_string) {
            memcpy(safe_string, Data, Size);
            safe_string[Size] = '\0';
        }
    }

    /* Fuzz curl_mime_headers with take_ownership=1 */
    struct curl_slist *headers1 = NULL;
    if (safe_string) {
        headers1 = curl_slist_append(headers1, safe_string);
        if (headers1) {
            curl_mime_headers(part, headers1, 1); /* Take ownership */
        }
    }

    /* Fuzz curl_mime_headers with take_ownership=0 and then free */
    struct curl_slist *headers2 = NULL;
    if (safe_string) {
        headers2 = curl_slist_append(headers2, safe_string);
        if (headers2) {
            curl_mime_headers(part, headers2, 0); /* Do not take ownership */
            curl_slist_free_all(headers2);
        }
    }

    /* Fuzz curl_mime_filename */
    if (part && safe_string) {
        curl_mime_filename(part, safe_string);
    }

    /* Fuzz curl_mime_filedata */
    if (part) {
        curl_mime_filedata(part, dummy_file);
    }

    /* Fuzz curl_easy_strerror with various error codes */
    /* Define a static array of known error codes to avoid incomplete type issues */
    CURLcode test_codes[] = {
        CURLE_OK,
        CURLE_BAD_FUNCTION_ARGUMENT,
        CURLE_OUT_OF_MEMORY,
        CURLE_READ_ERROR,
        CURLE_COULDNT_CONNECT,
        CURLE_WRITE_ERROR,
        CURLE_OPERATION_TIMEDOUT,
        CURLE_URL_MALFORMAT,
        CURLE_NOT_BUILT_IN
    };
    size_t num_codes = sizeof(test_codes) / sizeof(test_codes[0]);
    for (size_t i = 0; i < num_codes; i++) {
        const char *error_str = curl_easy_strerror(test_codes[i]);
        (void)error_str; /* Suppress unused variable warning */
    }

    /* Cleanup */
    if (safe_string) {
        free(safe_string);
    }

    /* Clean up mime and part */
    curl_mime_free(mime);

    /* Clean up dummy file */
    remove(dummy_file);

    return 0;
}