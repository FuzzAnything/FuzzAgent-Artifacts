// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

/* Helper to safely create a mime part for fuzzing */
static curl_mimepart *create_mime_part(void) {
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) return NULL;
    
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return NULL;
    }
    
    return part;
}

/* Helper to clean up mime structures */
static void cleanup_mime(curl_mimepart *part) {
    /* curl_mimepart is an opaque type in public API, we must free via the parent mime handle.
     * We cannot access part->parent directly. Instead, we store the mime handle or rely on
     * curl_mime_free being called on the parent. Since we don't have a direct way to get
     * the parent from a part in the public API without internal headers, we need to adjust
     * our approach. We will create the mime, add a part, and then free the mime.
     * However, curl_mime_free frees the whole tree.
     * To fix the compilation error, we must not access internal fields of curl_mimepart.
     * We will change the logic to keep the mime handle if possible, or just free the mime
     * immediately after use if we don't need the part afterwards.
     * But curl_mime_free expects a curl_mime*, not curl_mimepart*.
     * The correct way to clean up is to free the curl_mime* that was created.
     * Since we only have the part here, we can't easily free it without the parent.
     * Let's refactor: create mime, add part, use part, free mime.
     * But the function signature expects part.
     * Actually, we can't get the parent from the part in the public API.
     * So we must change the helper to return the mime handle or handle cleanup differently.
     * Let's just create the mime, add part, use it, and free the mime inside the fuzzer loop
     * or pass the mime handle.
     * For simplicity and to fix the error: we will not access part->parent.
     * We will instead create a mime, add a part, use the part, and then free the mime.
     * But we need to know which mime to free.
     * Let's change the helper to return the mime handle, or just do everything in one place.
     * Actually, the standard pattern is:
     * curl_mime *mime = curl_mime_init(NULL);
     * curl_mimepart *part = curl_mime_addpart(mime);
     * ... use part ...
     * curl_mime_free(mime);
     * So we cannot free just the part. We must free the mime.
     * The cleanup function needs to know the mime handle.
     * Let's remove the cleanup function that tries to access internal fields and just
     * handle the lifecycle in the main function or pass the mime handle.
     */
    (void)part; /* Unused in this fixed version */
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test curl_global_trace with fuzzed input */
    /* Ensure null termination for string functions */
    char *config_str = malloc(Size + 1);
    if (config_str) {
        memcpy(config_str, Data, Size);
        config_str[Size] = '\0';
        curl_global_trace(config_str);
        free(config_str);
    }

    /* Test curl_easy_strerror with various error codes derived from input */
    if (Size >= 1) {
        CURLcode error_code = (CURLcode)Data[0];
        const char *str = curl_easy_strerror(error_code);
        (void)str; /* Use the result to avoid unused warning */
    }

    /* Test MIME functions */
    /* Create a mime structure to get a valid part */
    curl_mime *mime = curl_mime_init(NULL);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Test curl_mime_filename */
            if (Size >= 2) {
                char *filename = malloc(Size + 1);
                if (filename) {
                    memcpy(filename, Data, Size);
                    filename[Size] = '\0';
                    curl_mime_filename(part, filename);
                    free(filename);
                }
            }
            
            /* Test curl_mime_name */
            if (Size >= 2) {
                char *name = malloc(Size + 1);
                if (name) {
                    memcpy(name, Data, Size);
                    name[Size] = '\0';
                    curl_mime_name(part, name);
                    free(name);
                }
            }
            
            /* Test with NULL to clear name */
            curl_mime_name(part, NULL);
        }
        /* Free the entire mime structure which includes the part */
        curl_mime_free(mime);
    }

    /* Test with NULL part pointer to trigger CURLE_BAD_FUNCTION_ARGUMENT */
    curl_mime_filename(NULL, "test.txt");
    curl_mime_name(NULL, "test_name");

    return 0;
}