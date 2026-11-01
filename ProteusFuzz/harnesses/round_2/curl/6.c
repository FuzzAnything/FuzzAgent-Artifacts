// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create an easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Create a mime structure */
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Create a mime part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_mime_type */
    if (Size > 0) {
        /* Use input data as mime type string, ensure null termination */
        char *mime_type = (char *)malloc(Size + 1);
        if (mime_type) {
            memcpy(mime_type, Data, Size);
            mime_type[Size] = '\0';
            curl_mime_type(part, mime_type);
            /* Also test with NULL */
            curl_mime_type(part, NULL);
            free(mime_type);
        }
    }

    /* Fuzz curl_mime_name */
    if (Size > 0) {
        char *name = (char *)malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            curl_mime_name(part, name);
            /* Also test with NULL */
            curl_mime_name(part, NULL);
            free(name);
        }
    }

    /* Fuzz curl_mime_encoder */
    const char *encodings[] = {"base64", "quoted-printable", NULL, "invalid-encoding"};
    for (int i = 0; encodings[i] != NULL; i++) {
        curl_mime_encoder(part, encodings[i]);
    }
    /* Test with NULL to remove encoder */
    curl_mime_encoder(part, NULL);

    /* Fuzz curl_easy_reset */
    curl_easy_reset(easy_handle);

    /* Fuzz curl_multi_get_handles */
    /* Add easy handle to multi handle to ensure non-empty array */
    curl_multi_add_handle(multi_handle, easy_handle);
    
    CURL **handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        /* Iterate through the array until NULL is found */
        for (int i = 0; handles[i] != NULL; i++) {
            /* Just access to ensure validity, no operation needed */
            (void)handles[i];
        }
        curl_free(handles);
    }

    /* Cleanup */
    curl_mime_free(mime);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}