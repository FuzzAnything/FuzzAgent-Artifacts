// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl MIME functions */

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a MIME handle */
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Create a sub-MIME handle for testing subparts */
    curl_mime *submime = curl_mime_init(curl);
    if (!submime) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Add parts to the main MIME handle */
    curl_mimepart *part1 = curl_mime_addpart(mime);
    if (part1) {
        /* Set data for part1 using fuzz input */
        if (Size > 0) {
            curl_mime_data(part1, (char *)Data, Size);
        }
        /* Set name */
        curl_mime_name(part1, "fuzz_data");
        /* Set filename */
        curl_mime_filename(part1, "fuzz_file.txt");
        /* Set content type */
        curl_mime_type(part1, "application/octet-stream");
    }

    /* Add parts to the sub-MIME handle */
    curl_mimepart *subpart1 = curl_mime_addpart(submime);
    if (subpart1) {
        if (Size > 1) {
            curl_mime_data(subpart1, (char *)(Data + 1), Size > 1 ? Size - 1 : 0);
        }
        curl_mime_name(subpart1, "sub_fuzz_data");
    }

    /* Test curl_mime_subparts with valid part and submime */
    if (part1 && submime) {
        curl_mime_subparts(part1, submime);
    }

    /* Add another part to test multiple parts */
    curl_mimepart *part2 = curl_mime_addpart(mime);
    if (part2) {
        curl_mime_data(part2, "static_data", 11);
        curl_mime_name(part2, "static_name");
    }

    /* 
     * Important: When a submime is attached to a part in 'mime' via curl_mime_subparts,
     * freeing 'mime' will recursively free the submime.
     * Therefore, we must NOT free 'submime' separately after it has been attached.
     * We only free 'mime', which cleans up everything including 'submime'.
     */
    curl_mime_free(mime);
    /* mime is now freed, and submime was freed as part of it. 
       Do not call curl_mime_free(submime) again. */

    /* Test curl_mime_free with NULL handle (safe) */
    curl_mime_free(NULL);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}