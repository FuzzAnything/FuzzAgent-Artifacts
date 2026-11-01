// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a dummy file for testing curl_mime_filedata
    create_dummy_file("./dummy_file", Data, Size);

    // Initialize curl handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a basic URL to avoid immediate errors during perform
    if (curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1") != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Set timeout to avoid hanging
    if (curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L) != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Prepare post data buffer (ensure it's null-terminated)
    size_t post_size = Size < 1024 ? Size : 1024;
    char *post_data = malloc(post_size + 1);
    if (!post_data) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    memcpy(post_data, Data, post_size);
    post_data[post_size] = '\0';

    // Set post data
    if (curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data) != CURLE_OK) {
        free(post_data);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Create MIME structure for testing mime functions
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        free(post_data);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Add a part to the MIME structure
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
        // Test curl_mime_type
        if (Size > 0) {
            char mime_type[64];
            size_t mime_len = Size < 63 ? Size : 63;
            memcpy(mime_type, Data, mime_len);
            mime_type[mime_len] = '\0';
            curl_mime_type(part, mime_type);
        } else {
            curl_mime_type(part, "text/plain");
        }

        // Test curl_mime_filedata
        curl_mime_filedata(part, "./dummy_file");
    }

    // Set MIME data for the request
    if (curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime) != CURLE_OK) {
        curl_mime_free(mime);
        free(post_data);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Perform the request
    curl_easy_perform(curl);

    // Test curl_easy_getinfo
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    // Test curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Test curl_global_trace (with a safe config string)
    char trace_config[32];
    size_t trace_len = Size < 31 ? Size : 31;
    memcpy(trace_config, Data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);

    // Cleanup
    curl_mime_free(mime);
    free(post_data);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}