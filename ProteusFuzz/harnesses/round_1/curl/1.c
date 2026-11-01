// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    // Initialize the main MIME handle
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        return 0;
    }

    // Add a part to the main MIME handle
    curl_mimepart *part1 = curl_mime_addpart(mime);
    if (part1) {
        // Fuzz filename
        if (Size > 0) {
            char *filename = malloc(Size + 1);
            if (filename) {
                memcpy(filename, Data, Size);
                filename[Size] = '\0';
                curl_mime_filename(part1, filename);
                free(filename);
            }
        }

        // Fuzz encoder with valid and invalid names
        const char *encodings[] = {"base64", "quoted-printable", "7bit", "8bit", "binary", NULL};
        for (int i = 0; encodings[i] != NULL; i++) {
            curl_mime_encoder(part1, encodings[i]);
        }
        curl_mime_encoder(part1, NULL);
    }

    // Create a nested MIME structure for subparts
    // Note: We must ensure this is not added as a subpart to 'part1' if we plan to free 'mime' first,
    // or we must free 'submime' before 'mime' if it is a subpart.
    // To avoid use-after-free, we will NOT link 'submime' as a subpart of 'part1' in this specific flow
    // if we free 'mime' first. Instead, we test the API calls independently or ensure correct order.
    
    // Scenario A: Test subparts linkage correctly
    curl_mime *submime = curl_mime_init(easy);
    if (submime) {
        curl_mimepart *subpart = curl_mime_addpart(submime);
        if (subpart) {
            if (Size > 0) {
                char *data = malloc(Size + 1);
                if (data) {
                    memcpy(data, Data, Size);
                    data[Size] = '\0';
                    curl_mime_filename(subpart, data);
                    free(data);
                }
            }
        }

        // Link submime to part1 ONLY if part1 exists
        if (part1) {
            curl_mime_subparts(part1, submime);
            // CRITICAL: Since submime is now a child of part1 (which is in mime),
            // we MUST NOT call curl_mime_free(submime) explicitly here.
            // Freeing 'mime' will recursively free 'submime'.
            // We set submime to NULL to prevent double free later.
            submime = NULL; 
        }
    }

    // Add another part to the main mime
    curl_mimepart *part2 = curl_mime_addpart(mime);
    if (part2) {
        curl_mime_filename(part2, "");
        curl_mime_encoder(part2, "invalid_encoding");
    }

    // Cleanup
    // Free the main mime handle. This will recursively free submime if it was attached.
    curl_mime_free(mime);
    
    // If submime was not attached (submime != NULL), free it now.
    if (submime) {
        curl_mime_free(submime);
    }

    curl_easy_cleanup(easy);

    return 0;
}