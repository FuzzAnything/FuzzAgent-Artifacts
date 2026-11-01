// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
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

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file for file-based operations */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        /* Write some dummy data to the file */
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Initialize curl_slist for headers */
    struct curl_slist *headers = NULL;
    
    /* Ensure we have a null-terminated string for slist_append */
    char *safe_data = NULL;
    if (Size > 0) {
        safe_data = (char *)malloc(Size + 1);
        if (safe_data) {
            memcpy(safe_data, Data, Size);
            safe_data[Size] = '\0';
        }
    }

    if (safe_data) {
        /* Build header list */
        headers = curl_slist_append(headers, safe_data);
        free(safe_data);
    }

    /* Create a MIME structure and part */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        curl_slist_free_all(headers);
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_slist_free_all(headers);
        return 0;
    }

    /* Fuzz curl_mime_headers */
    if (part && headers) {
        /* Try with take_ownership = 1 and 0 */
        curl_mime_headers(part, headers, 1);
        /* Note: if take_ownership=1, headers is managed by libcurl, so don't free it manually here */
    }

    /* Fuzz curl_mime_filename */
    if (part) {
        /* Use a safe filename derived from input or a constant */
        char safe_filename[256];
        size_t fn_len = (Size > 255) ? 255 : Size;
        memcpy(safe_filename, Data, fn_len);
        safe_filename[fn_len] = '\0';
        
        curl_mime_filename(part, safe_filename);
    }

    /* Fuzz curl_mime_filedata */
    if (part) {
        curl_mime_filedata(part, dummy_file);
    }

    /* Fuzz curl_easy_strerror with various error codes */
    CURLcode test_errors[] = {
        CURLE_OK,
        CURLE_BAD_FUNCTION_ARGUMENT,
        CURLE_OUT_OF_MEMORY,
        CURLE_READ_ERROR,
        CURLE_COULDNT_CONNECT,
        0, /* Test with 0 which is CURLE_OK */
        -1 /* Test with invalid code */
    };
    
    for (size_t i = 0; i < sizeof(test_errors) / sizeof(test_errors[0]); i++) {
        const char *str = curl_easy_strerror(test_errors[i]);
        (void)str; /* Use the result to avoid unused variable warning */
    }

    /* Cleanup */
    curl_mime_free(mime);
    
    /* Only free headers if we didn't pass ownership to curl_mime_headers */
    /* In the logic above, we passed take_ownership=1, so headers is freed by curl_mime_free */
    /* If we were to test take_ownership=0, we would need to free headers here */
    
    /* Clean up dummy file */
    remove(dummy_file);

    return 0;
}